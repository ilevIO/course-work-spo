#include "broadcastnetworkcontroller.h"
#include <math.h>
#include <QDebug>
#include <thread>
#include <map>

using namespace std;

#define SIMD

BroadcastNetworkController::BroadcastNetworkController(bool server, QString ip)
    : recieveThread(this)
{
    sendSound.call = false;
    sendSound.disconect = false;
    sendSound.sending = true;
    sendSound.frequency = -1;

    callingSound.call = true;
    callingSound.sending= true;
    callingSound.disconect = false;
    callingSound.frequency = -1;
    for (int i = 0; i < MESSAGE_SIZE; i++)
        callingSound.audio_data[i] = char(50 * sin(i * 1024));

    this->n_exit = true;

    QAudioDeviceInfo infoInput = QAudioDeviceInfo::defaultInputDevice();
    QAudioDeviceInfo infoOutput = QAudioDeviceInfo::defaultOutputDevice();

    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(RATE_BYTES);
    format.setSampleType(RATE_BYTES == 16 ? QAudioFormat::SignedInt : QAudioFormat::UnSignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    this->audioInput = new QAudioInput(infoInput, format);
    this->audioOutput = new QAudioOutput(infoOutput, format);

    this->is_call = false;
    this->is_send = false;
    this->is_config = false;

    this->szum_level = 2;


    recieveThread.start();
}


void BroadcastNetworkController::run()
{
//    audioInput->setBufferSize(1024);
//    audioOutput->setBufferSize(1024);

    Message msg;
    memset(&msg, '\0', sizeof(Message));
    msg.frequency = -1;

     auto audioOut = this->audioOutput->start();
     auto audioIn = this->audioInput->start();

     bool switched = false;
     bool call = false;

     qDebug("network thread is run");

     while(n_exit)
     {
         this->change_state.lock();

         if (this->is_config)
         {
             if (audioOutput->state() == QAudio::IdleState)
                qDebug() << "idle";
             if (this->is_send)
             {
                 if (this->is_call)
                 {
                     if (call != true)
                     {
                         switched = true;
                     }
                     call = true;

                     callingSound.id = this->counter.getId();
                     client.send(callingSound);
                    // semaphore.lock();

                     audioOut->write(callingSound.audio_data, MESSAGE_SIZE);

                 }
                 else
                 {
                     if (call != false)
                     {
                         switched = true;
                     }
                     call = false;
                     audioIn->read(sendSound.audio_data, MESSAGE_SIZE);

                     sendSound.id = this->counter.getId();
                     client.send(sendSound);

                     audioOut->write(sendSound.audio_data, MESSAGE_SIZE);
                 }
             }
             else
             {
                 prepareToAudioOutput(recieveSound);
                 audioOut->write(recieveSound.audio_data, MESSAGE_SIZE);

//                 client.send(msg);

                 if (recieveSound.call)
                 {
                     if (call == false)
                     {
                         switched = true;
                     }
                     call = true;
                     recieveSound.call = false;
                 }
                 else
                 {
                     if (call == true)
                     {
                         switched = true;
                     }
                     call = false;
                 }
             }
         }
         else
         {
//             client.send(msg);
             //semaphore.unlock();
             //semaphore.lock();
            // not recieve not send (power off)
         }
         int frequency = this->recieveSound.frequency;
         memset(&this->recieveSound, '\0', sizeof(Message));
         recieveSound.frequency = frequency;
         this->change_state.unlock();

         if(switched)
         {
             switched = false;
             if (call)
             {
                 emit set_call();
             }
             else
             {
                 emit reset_call();
             }
         }
     }
}

Message BroadcastNetworkController::getSzum()
{
    Message message;
    for (int i = 0; i < MESSAGE_SIZE; i++)
        message.audio_data[i] = rand() % 100;

    return message;
}

void BroadcastNetworkController::prepareToAudioOutput(Message &msg)
{
    Message szum = getSzum();
    __m128i *destiny, *source;
    switch (szum_level) {
    case 0:
        return;
    case 1:
        for (int i = 0; i < MESSAGE_SIZE; i++)
            szum.audio_data[i] *= 0.1;
#ifndef SIMD
        for (int i = 0; i < MESSAGE_SIZE / 8; i++)
        {
            reinterpret_cast<long long*>(msg.audio_data)[i] +=
                    reinterpret_cast<long long*>(szum.audio_data)[i];
        }
#else
        destiny = reinterpret_cast<__m128i*>(msg.audio_data);
        source = reinterpret_cast<__m128i*>(szum.audio_data);
        for (int i = 0; i < MESSAGE_SIZE / 16; i++)
        {
            destiny[i] += source[i];
        }
#endif
//            szum.audio_data[i] *= 0.1;
//            msg.audio_data[i] += szum.audio_data[i];
        return;
    case 2:
        for (int i = 0; i < MESSAGE_SIZE; i++)
            szum.audio_data[i] *= 0.5;
#ifndef SIMD
        for (int i = 0; i < MESSAGE_SIZE / 8; i++)
        {
            reinterpret_cast<long long*>(msg.audio_data)[i] +=
                    reinterpret_cast<long long*>(szum.audio_data)[i];
        }
#else
        destiny = reinterpret_cast<__m128i*>(msg.audio_data);
        source = reinterpret_cast<__m128i*>(szum.audio_data);
        for (int i = 0; i < MESSAGE_SIZE / 16; i++)
        {
            destiny[i] += source[i];
        }
#endif
        return;
    }
}


void BroadcastNetworkController::config_send(int frequency)
{
    this->change_state.lock();

    this->is_config = true;
    this->is_send = true;

    sendSound.frequency = frequency;
    callingSound.frequency = frequency;

    this->change_state.unlock();
}

void BroadcastNetworkController::config_listen(int frequency)
{
    this->change_state.lock();

    this->is_config = true;
    this->is_send = false;

    recieveSound.frequency = frequency;

    this->change_state.unlock();
}

void BroadcastNetworkController::config_kill()
{
    this->change_state.lock();

    this->is_config = false;

    this->change_state.unlock();
}

void BroadcastNetworkController::call_on()
{
    this->change_state.lock();

    this->is_call = true;

    this->change_state.unlock();
}

void BroadcastNetworkController::call_off()
{
    this->change_state.lock();

    this->is_call = false;

    this->change_state.unlock();
}

void BroadcastNetworkController::setVolume(char level)
{
    change_state.lock();
    this->audioOutput->setVolume(qreal(level) / 100);
    change_state.unlock();
}

void BroadcastNetworkController::setSzumLevel(float level)
{
    change_state.lock();
    this->szum_level = level;
    change_state.unlock();
}

void BroadcastNetworkController::setRecordVolume(char level)
{
    change_state.lock();
    this->audioInput->setVolume(qreal(level) / 100);
    change_state.unlock();
}

BroadcastNetworkController::~BroadcastNetworkController()
{
    delete audioInput;
    delete audioOutput;

    this->n_exit = false;
}



void BroadcastNetworkController::RecieveThread::run()
{
    Message temp;

    QAudioDeviceInfo infoOutput = QAudioDeviceInfo::defaultOutputDevice();

    map<int, QIODevice*> outputs;

//    int launched = thisContr->semaphore.getLaunched();
//    for (int i = 0; i < launched; i++)
    while(thisContr->n_exit)
    {
//        for (int i = 0; i < thisContr->semaphore.getLaunched(); i++)
//        int times = thisContr->semaphore.getNeedToAccept();
//        for (int i = 0; i < times; i++)
//        {

            thisContr->client.recieve(temp);
            thisContr->change_state.lock();

            if (
                    thisContr->is_config && !thisContr->is_send,
                    thisContr->recieveSound.frequency == temp.frequency &&
                    temp.frequency != -1
            )
            {
                if (temp.call)
                        thisContr->recieveSound.call = true;
                if (outputs.find(temp.id) == outputs.end())
                    outputs[temp.id] = (new QAudioOutput(infoOutput, thisContr->format))->start();
                outputs[temp.id]->write(temp.audio_data, MESSAGE_SIZE);
            }
/*
            if (thisContr->is_send)
            {

            }
            else
            {
                if (thisContr->is_config)
                {
                    if (thisContr->recieveSound.frequency == temp.frequency && temp.frequency != -1)
                    {
                        if (temp.call && !thisContr->recieveSound.call)
                        {
                            memcpy(&thisContr->recieveSound, &temp, sizeof(Message));
                        }
                        else if (!thisContr->recieveSound.call)
                        {
#ifndef SIMD
                        for (int i = 0; i < MESSAGE_SIZE / 8; i++)
                            reinterpret_cast<long long*>(thisContr->recieveSound.audio_data)[i] +=
                                reinterpret_cast<long long*>(temp.audio_data)[i];
#else
                            __m128i *destiny = reinterpret_cast<__m128i*>(thisContr->recieveSound.audio_data);
                            __m128i *source = reinterpret_cast<__m128i*>(temp.audio_data);
                            for (int i = 0; i < MESSAGE_SIZE / 16; i++)
                            {
                                destiny[i] += source[i];
                            }
#endif
                        }

                    }
                }
            }
//            thisContr->change_state.unlock();
//        }
//        thisContr->semaphore.unlock();
*/
            thisContr->change_state.unlock();
    }
}

BroadcastNetworkController::RecieveThread::RecieveThread(BroadcastNetworkController *ptr)
{
    thisContr = ptr;
}
