#include "r123bpstatecontroller.h"

R123BPStateController::R123BPStateController()
{
    this->pit_launched = false;
    this->ziem_connected = false;
    this->pit_rst_launched = false;
}

void R123BPStateController::actionResolver(R123BPStateController::Action action)
{
    switch (action) {
    case PIT:
        this->pit_launched = !this->pit_launched;
        break;
    case ZIEM:
        this->ziem_connected = ! this->ziem_connected;
        break;
    case PIT_RST:
        this->pit_rst_launched = ! this->pit_rst_launched;
    }
}

bool R123BPStateController::canTouch()
{
    return this->ziem_connected;
}

bool R123BPStateController::isGood()
{
    return this->pit_launched && this->pit_rst_launched;
}


