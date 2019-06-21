#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H
#include "model/client.h"
#include "model/server.h"
#include "model/message.h"
#include <controller/networkcontroller/abstractnetworkcontroller.h>
#include <QThread>
#include <QAudioInput>
#include <QAudioOutput>


class NetworkController : public AbstractNetworkController
{
    Q_OBJECT

    Server * server;
    Client * client;

    bool n_exit;

    QMutex change_state;

    Message nothing;
    Message sendSound;
    Message callingSound;
    Message recieveSound;

    bool is_send;
    bool is_call;
    bool is_config;

    QAudioInput * audioInput;
    QAudioOutput * audioOutput;

    float szum_level;

    void run() override;

    Message getSzum();

    void prepareToAudioOutput(Message& msg);

public:
    NetworkController(bool server, QString ip = "127.0.0.1");

    void config_send(int frequency) override;
    void config_listen(int frequency) override;
    void config_kill() override;

    void call_on() override;
    void call_off() override;

    void setVolume(char level) override;
    void setSzumLevel(float level) override;
    void setRecordVolume(char level) override;

    ~NetworkController();

signals:

//    void set_call();
//    void reset_call();
};

#endif // NETWORKCONTROLLER_H
