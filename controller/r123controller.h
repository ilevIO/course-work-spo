#pragma once

#include "controller/networkcontroller/abstractnetworkcontroller.h"
#include "controller/r123statecontroller.h"


class R123;

class R123Controller
{
    R123 * parent;
    AbstractNetworkController * networkController;
    R123StateController stateController;

    void setNetworkState(bool state);
public:
    R123Controller(R123 * parent, QString IP, bool is_server, AbstractNetworkController * networkController);

    void keyPressHandler(QKeyEvent * ev);
    void keyReleaseHandler(QKeyEvent * ev);

    void setR123Zpcz(int zpcz);
    void setNastr(bool nastr, int frequency = 0);
    void actionResolver(R123StateController::Action);
};

