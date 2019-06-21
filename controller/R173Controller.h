#pragma once

//#define BROAD_CAST

#include <QKeyEvent>
#include "controller/statecontroller.h"
#include "controller/networkcontroller.h"
#include "controller/broadcastnetworkcontroller.h"
#include "controller/networkcontroller/abstractnetworkcontroller.h"


class R173;

class R173Controller : public QObject {


    Q_OBJECT

    //SoundController soundController;
    StateController stateController;

    AbstractNetworkController& networkController;

    void setNetworkState(bool activate);

    bool is_config;

    R173 * parent;

    class Perest : public QThread
    {
        R173Controller * parent;

        void run() override;

    public:
        Perest(R173Controller * parent) : parent(parent) {}

    } perest;


public:

    R173Controller(R173 * parent, QString IP, bool is_server, AbstractNetworkController * networkController);
    enum Button {
      B1, B2, B3, B4, B5, B6, B7, B8, B9, B0, BTON,
      BCLS, BPOMIECHI, BSZUM, BPOWER, BPOWERLEVEL, BTABLE,
      BPIT, BANT, BTEL
    };
    void KeyPressHandler(QKeyEvent * ev);
    void ButtonPressed(Button b);
    void KeyReleaseHandler(QKeyEvent * ev);
    void buttonReleased(Button b);

    void changeVolume(char level);

    void destroy();

public slots:

    void set_ton();
    void reset_ton();

signals:
    void prd_on();
    void prd_off();

};
