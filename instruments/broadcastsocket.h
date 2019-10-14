#ifndef BROADCASTSOCKET_H
#define BROADCASTSOCKET_H

#ifdef _WIN32
#incldude <winsock2.h>
#else
#include <arpa/inet.h>
#define NULL nullptr
typedef int SOCKET;
#endif

class BroadcastSocket
{
    SOCKET reciever, sender;
    sockaddr_in addr, res_addr;

public:
    BroadcastSocket();

    void bind(char* addr, int port);


    void send(char* buffer, int len);
    void recieve(char* buffer, int max_len);
};

#endif // BROADCASTSOCKET_H
