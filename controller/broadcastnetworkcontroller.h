#ifndef BROADCASTNETWORCONTROLLER_H
#define BROADCASTNETWORCONTROLLER_H

#include <QAudioInput>
#include <QAudioOutput>
#include <QMutex>
#include <QThread>
#include "model/message.h"
#include "model/broadcastclient.h"
#include "instruments/webCounter.h"
#include "controller/networkcontroller/abstractnetworkcontroller.h"

class BroadcastNetworkController : public AbstractNetworkController
{
    Q_OBJECT

//
    WebCounter counter;
    BroadcastClient client;

    bool n_exit;

    bool is_send;
    bool is_call;
    bool is_config;

    QMutex change_state;

    Message sendSound;
    Message callingSound;
    Message recieveSound;


    void run() override;
    Message getSzum();

    QAudioInput * audioInput;
    QAudioOutput * audioOutput;

    char szum_level;

    void prepareToAudioOutput(Message& msg);

    friend class RecieveThread;

    class RecieveThread : public QThread
    {
        BroadcastNetworkController* thisContr;
    public:
        void run() override;
        RecieveThread(BroadcastNetworkController *ptr);
    } recieveThread;

    QAudioFormat format;
    QAudioDeviceInfo infoOutput;

public:
    BroadcastNetworkController(bool server, QString ip = "127.0.0.1");

    void config_send(int frequency) override;
    void config_listen(int frequecy) override;
    void config_kill() override;

    void call_on() override;
    void call_off() override;

    void setVolume(char level) override;
    void setSzumLevel(float level) override;
    void setRecordVolume(char level) override;

    ~BroadcastNetworkController();
signals:

    //void set_call();
    //void reset_call();

public slots:
};

#endif // BROADCASTNETWORCONTROLLER_H
