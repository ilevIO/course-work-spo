#include "controller/r123bpstatecontroller.h"
#include "controller/r123rststatecontroller.h"
#include "controller/r123vsuastatecontroller.h"
#include <QKeyEvent>

#ifndef R123STATECONTROLLER_H
#define R123STATECONTROLLER_H

class R123;

class R123StateController
{
    R123BPStateController blockPit;
    R123RSTStateController rst;
    R123VSUAStateController vsua;
    R123 * parent;

public:
    R123StateController();

    enum Action {
        BP_ZIEM, BP_PIT, BP_PIT_RST,
        R123_PIT, R123_POWER, R123_SZKALA, R123_ANTENA,
    };

    void actionResolver(Action action);
    bool BP_canTouch();
    bool isConnected();
    int connectedOn();
    bool isSzkalaOn();

    bool r123_isPowerOn();

};

#endif // R123STATECONTROLLER_H
