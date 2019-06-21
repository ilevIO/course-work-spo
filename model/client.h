#pragma once

#include <QString>
#include "model/message.h"
#include "instruments/socket.h"


class Client {

    Socket socket;

public:
    Client(QString ip_address);

    void send(Message& msg);
    void recieve(Message& msg);

    ~Client();
};
