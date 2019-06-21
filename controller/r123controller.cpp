#include "r123controller.h"
#include "view/r123.h"
#include <QDebug>

void R123Controller::setNetworkState(bool state)
{

}

R123Controller::R123Controller(R123 * parent, QString IP, bool is_server, AbstractNetworkController * networkController) :
    parent(parent), networkController(networkController)
{

}

void R123Controller::keyPressHandler(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Space)
        setNetworkState(true);
}

void R123Controller::keyReleaseHandler(QKeyEvent *ev)
{
    if (!ev || ev->key() == Qt::Key_Space)
        setNetworkState(false);
}

void R123Controller::setR123Zpcz(int zpcz)
{
    qDebug() << int(zpcz);
}

void R123Controller::setNastr(bool nastr, int frequency)
{
    if (nastr) qDebug() << "nastr";
}

void R123Controller::actionResolver(R123StateController::Action action)
{
    this->stateController.actionResolver(action);
    switch (action) {
    case R123StateController::BP_ZIEM:
        this->parent->switch_BP_canTouch(this->stateController.BP_canTouch());
        break;
    default:
        break;
    }
    parent->setPowerState(this->stateController.r123_isPowerOn());
    parent->setLineState(stateController.isSzkalaOn());
}
