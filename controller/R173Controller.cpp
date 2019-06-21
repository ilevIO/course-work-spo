#include <controller/R173Controller.h>
#include <QDebug>
#include <Qt>
#include "view/r173.h"
#include <QThread>
#include <controller/networkcontroller/abstractnetworkcontroller.h>


R173Controller::R173Controller(R173 *parent, QString IP, bool is_server, AbstractNetworkController * networkController) :
    networkController(*networkController), perest(this)
{
    this->parent = parent;
    this->is_config = false;


    QObject::connect(networkController, SIGNAL(set_call()), this, SLOT(set_ton()), Qt::QueuedConnection);
    QObject::connect(networkController, SIGNAL(reset_call()), this, SLOT(reset_ton()), Qt::QueuedConnection);


    QObject::connect(this, SIGNAL(prd_on()), this->parent, SLOT(perest()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(prd_off()), this->parent, SLOT(perest()), Qt::QueuedConnection);
}

void R173Controller::KeyPressHandler(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Space)
        setNetworkState(true);
}

void R173Controller::ButtonPressed(R173Controller::Button b)
{
    int state = 0;
    switch(b){
    case B0:
        state = this->stateController.numberButtonPressed(0);
        break;
    case B1:
        state = this->stateController.numberButtonPressed(1);
        break;
    case B2:
        state = this->stateController.numberButtonPressed(2);
        break;
    case B3:
        state = this->stateController.numberButtonPressed(3);
        break;
    case B4:
        state = this->stateController.numberButtonPressed(4);
        break;
    case B5:
        state = this->stateController.numberButtonPressed(5);
        break;
    case B6:
        state = this->stateController.numberButtonPressed(6);
        break;
    case B7:
        state = this->stateController.numberButtonPressed(7);
        break;
    case B8:
        state = this->stateController.numberButtonPressed(8);
        break;
    case B9:
        state = this->stateController.numberButtonPressed(9);
        break;
    case BCLS:
        this->stateController.clsPresed();
        break;
    case BPOMIECHI:
        this->networkController.setSzumLevel(
            this->stateController.pomiechiIsPressed()
        );
        break;
    case BPOWER:
        this->stateController.powerIsPressed();
        break;
    case BSZUM:
        this->networkController.setSzumLevel(
            this->stateController.szumIsPressed()
        );
        break;
    case BPOWERLEVEL:
        this->networkController.setRecordVolume(
            this->stateController.powerLevelIsPressed()
        );
        break;
    case BTABLE:
        this->stateController.tableSwitcher();
        break;
    case BTON:
        this->networkController.call_on();
        break;
    case BTEL:
        this->stateController.telSwitcher();
        break;
    case BANT:
        this->stateController.antSwitcher();
        break;
    case BPIT:
        this->stateController.pitSwitcher();
        break;
    }

    if (state)
        this->perest.start();

    if (b != BTON)
        setNetworkState(false);

    this->parent->set_table(this->stateController.getTable());
}

void R173Controller::KeyReleaseHandler(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Space)
        this->setNetworkState(false);
}

void R173Controller::buttonReleased(R173Controller::Button b)
{
    switch (b) {
    case BTON:
        this->networkController.call_off();
        break;
    }
}

void R173Controller::changeVolume(char level)
{
    networkController.setVolume(level);
}

void R173Controller::destroy()
{
    delete &this->networkController;
}

void R173Controller::set_ton()
{
    this->parent->call_on();
}

void R173Controller::reset_ton()
{
    this->parent->call_off();
}

void R173Controller::setNetworkState(bool activate)
{
    static bool config = false;

    if (activate == true)
    {
        if (config == false && stateController.isReadyToRecieve())
        {
            parent->prd_on();
            config = true;
            this->networkController.config_send(stateController.getCurrentFrequency());
        }
        else
        {
            parent->prd_off();
            config = false;
            if (stateController.isReadyToRecieve())
                this->networkController.config_listen(stateController.getCurrentFrequency());
            else
                this->networkController.config_kill();
        }
    }
    else
    {
        parent->prd_off();
        if (this->stateController.isReadyToRecieve())
        {
            this->networkController.config_listen(this->stateController.getCurrentFrequency());
        }
        else
        {
            this->networkController.config_kill();
        }
        config = false;
    }

}


void R173Controller::Perest::run()
{
    emit parent->prd_on();

    this->msleep(3000);

    emit parent->prd_off();
}
