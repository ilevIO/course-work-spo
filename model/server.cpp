#include "model/server.h"
#include "model/client.h"

#ifndef _WIN32
#include <smmintrin.h>
#endif

Server::Server() :
    listenningThread(this),
    recieveThread(this)
{
    this->socket.bind(5000);
    this->exit_flag = true;

    listenningThread.start();
    recieveThread.start(QThread::TimeCriticalPriority);

}

void Server::close()
{
    this->exit_flag = false;
}

void Server::ListenningThread::run()
{
    while(parentServer->exit_flag)
    {
        parentServer->socket.listen();
        Socket * client = parentServer->socket.accept();

        qDebug("launched");

        parentServer->list_mutex.lock();
            parentServer->sockets.push_front(client);
            parentServer->messages.push_front(Message());
        parentServer->list_mutex.unlock();
    }
}

Server::ListenningThread::ListenningThread(Server *parent)
{
    this->parentServer = parent;
}

void Server::SendRecieveThread::run()
{

    while(parentServer->exit_flag)
    {
        parentServer->list_mutex.lock();

        auto clientPtr = parentServer->sockets.begin();
        auto messagePtr = parentServer->messages.begin();
        for(;messagePtr != parentServer->messages.end(); clientPtr++, messagePtr++)
        {
           if ((*clientPtr)->receive(reinterpret_cast<char*>(&*messagePtr),
                                     sizeof(Message)) == -1)
           {
               messagePtr->call = false;
               messagePtr->sending = false;
               messagePtr->disconect = true;
               messagePtr->frequency = -1;
           }
        }

        clientPtr = parentServer->sockets.begin();
        messagePtr = parentServer->messages.begin();
        for(;messagePtr != parentServer->messages.end(); clientPtr++, messagePtr++)
        {
            if(!messagePtr->sending && !messagePtr->disconect)
            {

                memset(messagePtr->audio_data, '\0', MESSAGE_SIZE);

                for (auto &message : parentServer->messages)
                {
                    if (message.sending && !message.disconect &&
                            message.frequency == messagePtr->frequency)
                    {
/*
                        for (int i = 0; i < MESSAGE_SIZE / 8; i++)
                        {
                            reinterpret_cast<long long *>(messagePtr->audio_data)[i] +=
                                    reinterpret_cast<long long *>(message.audio_data)[i];
                        }
*/

                        __m128i* destiny = reinterpret_cast<__m128i*>(messagePtr->audio_data);
                        __m128i* source  = reinterpret_cast<__m128i*>(message.audio_data);

                        for (int i = 0; i < MESSAGE_SIZE / 16; i++)
                        {
                            destiny[i] += source[i];
                        }

                        if (message.call)
                            messagePtr->call = true;
                    }
                }
            }
        }

        clientPtr = parentServer->sockets.begin();
        messagePtr = parentServer->messages.begin();

        for(;messagePtr != parentServer->messages.end(); clientPtr++, messagePtr++)
        {
            if(messagePtr->disconect)
            {

                qDebug("disconnect");
//                auto * socket = *clientPtr;
//                delete socket;
                parentServer->messages.remove(*messagePtr);
                parentServer->sockets.remove(*clientPtr);

                messagePtr = parentServer->messages.begin();
                clientPtr = parentServer->sockets.begin();
            }
        }

        clientPtr = parentServer->sockets.begin();
        messagePtr = parentServer->messages.begin();
        for(;messagePtr != parentServer->messages.end(); clientPtr++, messagePtr++)
        {
            (*clientPtr)->send(reinterpret_cast<char*>(&*messagePtr), sizeof(Message));
        }

        parentServer->list_mutex.unlock();
    }
}

Server::SendRecieveThread::SendRecieveThread(Server *parent)
{
    this->parentServer = parent;
}
