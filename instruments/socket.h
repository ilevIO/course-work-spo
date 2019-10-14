#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#ifdef _WIN32
#incldude <winsock2.h>
#else
#include <arpa/inet.h>
#define NULL nullptr
typedef int SOCKET;
#endif


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
