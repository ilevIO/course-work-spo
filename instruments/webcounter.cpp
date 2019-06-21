#include "webCounter.h"
#include <string>
#include <QDebug>
using namespace std;


void WebCounter::run()
{
    char msg[SEMAPHORE_MSG_LEN + 1];
    memset(msg, '\0', SEMAPHORE_MSG_LEN + 1);
    strcpy(msg, "new");
    this->socket_counter.send(msg, SEMAPHORE_MSG_LEN);
    this->socket_controller.send(msg, SEMAPHORE_MSG_LEN);
    bool first_new = false;
    while(this->n_exit)
    {
         memset(msg, '\0', SEMAPHORE_MSG_LEN);
         socket_counter.recieve(msg, SEMAPHORE_MSG_LEN);
         qDebug() << msg;
         if (string(msg) == "i am here")
         {
             this->launched++;
         }
         else if (string(msg) == "delete")
         {
             this->launched--;
             this->lockedValue--;
         }
         else if (string(msg) == "new")
         {
             if (first_new)
                 break;
             first_new = true;
             launched++;
         }
         qDebug() << "first cycle: " << launched;
    }

    this->launched++;
    memset(msg, '\0', SEMAPHORE_MSG_LEN + 1);
    strcpy(msg, "i am here");
    this->socket_counter.send(msg, SEMAPHORE_MSG_LEN);

    qDebug() << "non cycle: " << launched;

    while(this->n_exit)
    {
        memset(msg, '\0', SEMAPHORE_MSG_LEN);
        socket_counter.recieve(msg, SEMAPHORE_MSG_LEN);
        if (string(msg) == "delete")
        {
            this->launched--;
        }
        else if (string(msg) == "new")
        {
            this->launched++;
            memset(msg, '\0', SEMAPHORE_MSG_LEN + 1);
            strcpy(msg, "i am here");
            this->socket_counter.send(msg, SEMAPHORE_MSG_LEN);
        }
        qDebug() << msg;
        qDebug() << "second sycle: " << launched;
    }
}

WebCounter::WebCounter()
{
    this->socket_controller.bind(nullptr, 5001);
    this->socket_counter.bind(nullptr, 5002);
    this->launched = 0;
    lockedValue = 0;
    needToAccept = 0;
    this->n_exit = true;
    this->is_locked = false;
    this->id = clock();
    this->start();
}


