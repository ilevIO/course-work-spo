#include "socket.h"
#ifdef _WIN32
#include <winsock2.h>
#define INVALID_SOCK_VALUE( SOCK ) SOCK == INVALID_SOCKET
#else
#include <unistd.h>
#include <arpa/inet.h>
#define closesocket( A ) close( A )
#define INVALID_SOCK_VALUE( SOCK ) SOCK <= 0
#define SOCKET_ERROR -1
#endif
#include <QDebug>
#include <string.h>
//#pragma comment(lib, "ws2_32.lib")

Socket::Socket()
{

    if (INVALID_SOCK_VALUE((this->sock = socket(AF_INET, SOCK_STREAM, 0))))
        qDebug() << "error sock init";
    qDebug() << "sock is inited";
}

Socket::~Socket()
{
    closesocket(this->sock);
}

int Socket::connect(const char * ip_addr, int port)
{
    memset(&this->address, '\0', sizeof(this->address));
    this->address.sin_addr.s_addr = inet_addr(ip_addr);
    this->address.sin_family = AF_INET;

    this->address.sin_port = htons(port);

    if (::connect(
        this->sock,
        (sockaddr*)&this->address,
        sizeof(address)
        ) < 0)
    {
    qDebug() << "error connection";
    return 2;
    }
    qDebug() << "conetion is good";
    return 0;
}

int Socket::send(char* data, int len)
{
    if (::send(this->sock, data, len, 0) < 0)//is_server ? 0 : MESSAGE_SIZE) < 0)
        return 1;
    else
    return 0;
}
int Socket::receive(char* buffer, int len)
{
    int recv_size;
    recv_size = recv(this->sock, buffer, len, 0);
    if (recv_size == SOCKET_ERROR)
        return -1;
    else {
        return recv_size;
    }
}

int Socket::bind(int port)
{
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(port);

    auto resp = ::bind(this->sock, (sockaddr*)&this->address, sizeof(address));

    return resp < 0;
}
int Socket::listen() const
{
    qDebug() << "listenning";
    auto resp = ::listen(this->sock, 1);
    return resp < 0;
}
Socket * Socket::accept()
{
    Socket *sock = new Socket(6);
#ifdef _WIN32
    int size = sizeof(address);
#else
    unsigned int size = sizeof(address);
#endif
    sock->sock = ::accept(this->sock, (sockaddr*)&this->address, &size);

    qDebug() << (INVALID_SOCK_VALUE(sock->sock));

    return sock;
}

Socket::Socket(int)
{
}

void Socket::init() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif
}
void Socket::clear() {
#ifdef _WIN32
    WSACleanup();
#endif
}
