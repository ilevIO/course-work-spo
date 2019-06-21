#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <WinSock2.h>


class Socket
{
    SOCKET sock;
    sockaddr_in address;

    Socket(int);
public:

    static void init();
    static void clear();

    int send(char* data, int len);
    int receive(char* buffer, int len);
    int connect(const char * ip_addr, int port);
    int bind(int port);


    int listen() const;
    Socket * accept();

    Socket();
    ~Socket();
};

#endif // SOCKETCLIENT_H
