#include "r123rststatecontroller.h"

R123RSTStateController::R123RSTStateController()
{
    power_on = false;
    pit = false;
    szkala = false;
    pit_bp = false;
    nastr = false;
    antena = false;
}

void R123RSTStateController::actionResolver(R123RSTStateController::Action action)
{
    switch (action) {
    case PIT:
        pit = !pit;
        break;
    case SZKALA:
        szkala = !szkala;
        break;
    case POWER:
        power_on = !power_on;
        break;
    case PIT_BP:
        pit_bp = !pit_bp;
        break;
    case ANTENA:
        antena = !antena;
        break;
    }
}

void R123RSTStateController::setNastr(bool nastr)
{
    this->nastr = nastr;
}

bool R123RSTStateController::isReadyToNetwork()
{
    return nastr && isPowerLaunch() && antena;
}

bool R123RSTStateController::isPowerLaunch()
{
   return pit && power_on && pit_bp;
}

bool R123RSTStateController::isSzkalaOn()
{
    return power_on && pit && szkala && pit_bp;
}
