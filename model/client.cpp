#include "model/client.h"
#include <string>

Client::Client(QString ip_address)
{

    if (this->socket.connect(ip_address.toStdString().c_str(), 5000))
    {
        //throw std::string("Либо сервера не существует, либо проверьте подключение!");
    }
}

void Client::send(Message &msg)
{
    this->socket.send(reinterpret_cast<char*>(&msg), sizeof(msg));
}
void Client::recieve(Message &msg)
{
    if (this->socket.receive(reinterpret_cast<char*>(&msg), sizeof(msg)) == -1) {
        //becomeServer();
    }
}

Client::~Client()
{
    Message m;
    m.disconect = true;
    m.sending = false;

    send(m);
}
