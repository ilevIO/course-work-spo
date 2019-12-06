#include <QThread>
#ifndef ABSTRACTNETWORKCONTROLLER_H
#define ABSTRACTNETWORKCONTROLLER_H

class AbstractNetworkController : public QThread {

    Q_OBJECT

public:
    virtual void config_send(int frequency) = 0;
    virtual void config_listen(int frequecy) = 0;
    virtual void config_kill() = 0;

    virtual void call_on() = 0;
    virtual void call_off() = 0;

    virtual void setVolume(char level) = 0;
    virtual void setSzumLevel(float level) = 0;
    virtual void setRecordVolume(char level) = 0;

signals:

    void set_call();
    void reset_call();
};

#endif // ABSTRACTNETWORKCONTROLLER_H
