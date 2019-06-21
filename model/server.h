#pragma once
#include "instruments/socket.h"
#include <QThread>
#include <list>
#include <QMutex>
#include "model/message.h"


class Server
{

    bool exit_flag;

    QMutex list_mutex;

    Socket socket;

    std::list<Socket*> sockets;
    std::list<Message> messages;

    friend class ListenningThread;
    friend class SendRecieveThread;

    class ListenningThread : public QThread
    {
        Server * parentServer;
        void run() override;
    public:
        ListenningThread(Server* parent);

    } listenningThread;

    class SendRecieveThread : public QThread
    {

        Server * parentServer;
        void run() override;
    public:
        SendRecieveThread(Server * parent);

    } recieveThread;


    /*class SendThread : public QThread
    {
        Server * parentServer;
        void run() override;
    public:
        SendThread(Server * parent);
    } sendThread;  */
public:
    Server();

    void close();
};
