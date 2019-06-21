#ifndef BROADCASTCLIENT_H
#define BROADCASTCLIENT_H
#include "instruments/broadcastsocket.h"
#include "model/message.h"

class BroadcastClient
{
    BroadcastSocket socket;
public:
    BroadcastClient();

    void send(Message& message);
    void recieve(Message& message);
};

#endif // BROADCASTCLIENT_H
