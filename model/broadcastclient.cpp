#include "broadcastclient.h"

BroadcastClient::BroadcastClient()
{
//    this->socket.bind(NULL, 5000);
    this->socket.bind(NULL, 5000);
}

void BroadcastClient::send(Message &message)
{
    socket.send(reinterpret_cast<char*>(&message), sizeof(Message));
}

void BroadcastClient::recieve(Message &message)
{
    socket.recieve(reinterpret_cast<char*>(&message), sizeof(Message));
}
