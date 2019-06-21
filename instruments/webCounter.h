#ifndef WEBSEMAPHORE_H
#define WEBSEMAPHORE_H

#include <QThread>
#include <QMutex>
#include "instruments/broadcastsocket.h"

#define SEMAPHORE_MSG_LEN 9

class WebCounter : private QThread
{
       BroadcastSocket socket_controller, socket_counter;
       int launched, lockedValue, needToAccept;
       bool n_exit;
       bool is_locked;

       int id;

       void run() override;


public:
       WebCounter();


       int getLaunched()
       {
           return this->launched;
       }

       int getId()
       {
           return id;
       }

};
#endif // WEBSEMAPHORE_H
