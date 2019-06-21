#include "r123statecontroller.h"
#include "view/r123.h"

R123StateController::R123StateController()
{

}

void R123StateController::actionResolver(R123StateController::Action action)
{
    bool old_pit_state = blockPit.isGood();
    switch (action) {
       case BP_PIT:
        this->blockPit.actionResolver(R123BPStateController::PIT);
        break;
    case BP_ZIEM:
        this->blockPit.actionResolver(R123BPStateController::ZIEM);
        break;
    case BP_PIT_RST:
        this->blockPit.actionResolver(R123BPStateController::PIT_RST);
        break;
    case R123_PIT:
        this->rst.actionResolver(rst.PIT);
        break;
    case R123_POWER:
        this->rst.actionResolver(rst.POWER);
        break;
    case R123_SZKALA:
        this->rst.actionResolver(rst.SZKALA);
        break;
    case R123_ANTENA:
        this->rst.actionResolver(rst.ANTENA);
        break;
    }
    if (old_pit_state != blockPit.isGood())
    {
        this->rst.actionResolver(rst.PIT_BP);
    }

//    this->parent->setPowerState(this->rst.isPowerLaunch());
}

bool R123StateController::BP_canTouch()
{
    return this->blockPit.canTouch();
}

bool R123StateController::isConnected()
{
    return this->blockPit.isGood();
}

int R123StateController::connectedOn()
{
    return 30000;
}

bool R123StateController::isSzkalaOn()
{
    return this->rst.isSzkalaOn();
}


bool R123StateController::r123_isPowerOn()
{
    return this->rst.isPowerLaunch();
}


