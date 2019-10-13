#include "broadcastsocket.h"

#include <QDebug>

BroadcastSocket::BroadcastSocket()
{
    char broadcast = 'a';
/*    this->sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sender < 0)
    {
        qDebug() << "sock sender init error";
        return;
    }


    if (setsockopt(
        sender, SOL_SOCKET, SO_BROADCAST,
        &broadcast, sizeof(broadcast)
    ) < 0)
    {
        qDebug() << "sock sender option error";
        return;
    }
    qDebug() << "sock sender is init";
*/
    this->reciever = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (reciever < 0)
    {
        qDebug() << "sock reciever init error";
        return;
    }

    if (setsockopt(
        reciever, SOL_SOCKET, SO_BROADCAST,
        &broadcast, sizeof(broadcast)
    ) < 0)
    {
        qDebug() << "sock reciever option error";
        return;
    }

    if (setsockopt(
        reciever, SOL_SOCKET, SO_REUSEADDR,
        &broadcast, sizeof(broadcast)
    ) < 0)
    {
        qDebug() << "sock reciever option error";
        return;
    }

    qDebug() << "sock reciever is init";
}

void BroadcastSocket::bind(char *addr, int port)
{
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port);
    this->addr.sin_addr.s_addr = inet_addr("255.255.255.255");
          //  (addr ? inet_addr(addr) : INADDR_BROADCAST);

    this->res_addr.sin_family = AF_INET;
    this->res_addr.sin_port = htons(port);
    this->res_addr.sin_addr.s_addr = INADDR_ANY;
    if (::bind(this->reciever, (sockaddr*)&res_addr, sizeof(res_addr)) < 0)
        qDebug() << "bind error";

}

void BroadcastSocket::send(char *buffer, int len)
{
    if (sendto(this->reciever, buffer, len, 0,
               (sockaddr*)&this->addr, sizeof(addr)) < 0)
        qDebug() << "message isn't sent";
}

void BroadcastSocket::recieve(char *buffer, int max_len)
{
    static socklen_t size = sizeof(sockaddr_in);
    if (recvfrom(this->reciever, buffer, max_len, 0, (sockaddr*)&this->res_addr, &size) < 0)
        qDebug() << "error recieve";
}
