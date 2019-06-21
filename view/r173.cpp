#include "r173.h"
#include "ui_r173.h"
#include <controller/statecontroller.h>
#include <QDebug>
#include <ctime>
#include <math.h>
#include <QPainter>


void R173::keyPressEvent(QKeyEvent *ev)
{
    if (!ev->isAutoRepeat() && this->handle_key_events)
        this->controller.KeyPressHandler(ev);
}

void R173::keyReleaseEvent(QKeyEvent *ev)
{
    if (!ev->isAutoRepeat() && this->handle_key_events)
        this->controller.KeyReleaseHandler(ev);
}

void R173::setEnableToAll(bool anable)
{
    this->ui->ton->setEnabled(anable);
    this->ui->szum->setEnabled(anable);
    this->ui->power->setEnabled(anable);
    this->ui->button0->setEnabled(anable);
    this->ui->button1->setEnabled(anable);
    this->ui->button2->setEnabled(anable);
    this->ui->button3->setEnabled(anable);
    this->ui->button4->setEnabled(anable);
    this->ui->button5->setEnabled(anable);
    this->ui->button6->setEnabled(anable);
    this->ui->button7->setEnabled(anable);
    this->ui->button8->setEnabled(anable);
    this->ui->button9->setEnabled(anable);
    this->ui->buttonCls->setEnabled(anable);
    this->ui->pomiechi->setEnabled(anable);
    this->ui->powerLevel->setEnabled(anable);
    this->ui->tableSwitch->setEnabled(anable);

    handle_key_events = anable;
}

void R173::mousePressEvent(QMouseEvent *event)
{

    static int pos_x_zem = 1000;
    static int pos_x_ant = 881;

    if (event->x() > 0 && event->x() < 201 &&
        event->y() > 446 && event->y() < 600) // x>140, y < 537
    {
        this->telefon_is_pressed();
    }
    else if (event->x() > 0 && event->x() < 201 &&
             event->y() > 290 && event->y() < 450) // x > 140, y < 361
    {
        this->pitanije_is_pressed();
    }
    else if (event->x() > 850 && event->x() < pos_x_ant &&
             event->y() > 560 && event->y() < 621) // x < 881
    {
        if (pos_x_ant == 881)
            pos_x_ant = 1000;
        else
            pos_x_ant = 881;
        this->antenna_is_pressed();
    }
    else if (event->x() > 910 && event->x() < pos_x_zem &&
             event->y() > 570 && event->y() < 601) // x < 961
    {
        this->zeziemlinije_is_pressed();
    }

}

void R173::antenna_is_pressed()
{
    qDebug() << "antena";
    static bool antena_launched = false;
    static QPixmap on(":/res/p173-lno.png"), off;

    if (antena_launched)
    {
        this->ui->provod4_show->setPixmap(off);
        antena_launched = !antena_launched;
    }
    else
    {
        this->ui->provod4_show->setPixmap(on);
        antena_launched = !antena_launched;
    }
    controller.ButtonPressed(controller.BANT);
}

void R173::zeziemlinije_is_pressed()
{
    qDebug() << "zaziemlienije";
    static bool zazemleno = false;
    static QPixmap on(":/res/p173-lvo.png"), off;

    if (zazemleno)
    {
        this->ui->provod3_show->setPixmap(off);
        this->setEnableToAll(false);
        zazemleno = !zazemleno;
    }
    else
    {
        this->setEnableToAll(true);
        this->ui->provod3_show->setPixmap(on);
        zazemleno = !zazemleno;
    }
}

void R173::telefon_is_pressed()
{
    qDebug() << "telefon";
    static bool telefon_launched = false;
    static QPixmap on(":/res/p173-nlo.png"), off;

    if (telefon_launched)
    {
        this->ui->provod1_show->setPixmap(off);
        telefon_launched = !telefon_launched;
    }
    else
    {
        this->ui->provod1_show->setPixmap(on);
        telefon_launched = !telefon_launched;
    }
    controller.ButtonPressed(controller.BTEL);
}

void R173::pitanije_is_pressed()
{
    qDebug() << "pitanije";
    static bool zapitano = false;
    static QPixmap on(":/res/p173-nvo.png"), off;

    if (zapitano)
    {
        this->ui->provod2_show->setPixmap(off);
        zapitano = !zapitano;
    }
    else
    {
        this->ui->provod2_show->setPixmap(on);
        zapitano = !zapitano;
    }
    controller.ButtonPressed(controller.BPIT);
}


R173::R173(QString IP, bool is_server, AbstractNetworkController * networkController) :
    QMainWindow(nullptr),
    ui(new Ui::R173),
    controller(this, IP, is_server, networkController)
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    this->setFixedSize(992, 671);

    ui->setupUi(this);

    this->setEnableToAll(false);
    controller.changeVolume((24) * 100 / 124);
}

R173::~R173()
{
    //Socket::clear();
    delete ui;
}

void R173::set_table(StateController::TableInfo table)
{
    if (table.field[0] == -1)
        ui->field0->setStyleSheet("background-image: url(:/res/tableNubers/field_off.jpg);");
    else
        ui->field0->setStyleSheet("background-image: url(:/res/tableNubers/field" +
                                   QString::number(table.field[0]) + ".jpg);");

    if (table.field[1] == -1)
        ui->field1->setStyleSheet("background-image: url(:/res/tableNubers/field_off.jpg);");
    else
        ui->field1->setStyleSheet("background-image: url(:/res/tableNubers/field" +
                                   QString::number(table.field[1]) + ".jpg);");

    if (table.field[2] == -1)
        ui->field2->setStyleSheet("background-image: url(:/res/tableNubers/field_off.jpg);");
    else
        ui->field2->setStyleSheet("background-image: url(:/res/tableNubers/field" +
                                   QString::number(table.field[2]) + ".jpg);");

    if (table.field[3] == -1)
        ui->field3->setStyleSheet("background-image: url(:/res/tableNubers/field_off.jpg);");
    else
        ui->field3->setStyleSheet("background-image: url(:/res/tableNubers/field" +
                                   QString::number(table.field[3]) + ".jpg);");

    if (table.field[4] == -1)
        ui->field4->setStyleSheet("background-image: url(:/res/tableNubers/field_off.jpg);");
    else
        ui->field4->setStyleSheet("background-image: url(:/res/tableNubers/field" +
                                   QString::number(table.field[4]) + ".jpg);");

    if (table.field[5] == -1)
        ui->field5->setStyleSheet("background-image: url(:/res/tableNubers/field_off.jpg);");
    else
        ui->field5->setStyleSheet("background-image: url(:/res/tableNubers/field" +
                                  QString::number(table.field[5]) + ".jpg);");
}

void R173::prd_on()
{
    this->ui->prd->setStyleSheet("background-image: url(:/res/pered_o.jpg);");
}

void R173::prd_off()
{
    this->ui->prd->setStyleSheet("background-image: url(:/res/pered.jpg);");
}

void R173::wheelEvent(QWheelEvent *event)
{

    static QPixmap * volumePrdPxmap = new QPixmap(":/res/p-173m-round.png");
    static QPixmap * volumeSoundPxmap = new QPixmap(":/res/p-173m-round-sound.png");

    static int anglePrd = 0;
    static int angleSnd = 0;

    if (event->pos().rx() > 433 && event->pos().rx() < 464
         && event->pos().ry() > 377 && event->pos().ry() < 408)
    {

        QPixmap pixmap(*volumePrdPxmap);
        QMatrix rm;

        if (event->angleDelta().ry() > 0 && anglePrd < 32)
            anglePrd += 8;
        else if (event->angleDelta().ry() < 0 && anglePrd > -92)
            anglePrd -= 8;
        rm.rotate(anglePrd);

        pixmap = pixmap.transformed(rm);

        ui->volumePRM->setPixmap(QPixmap(pixmap.transformed(rm)));

    }
    if (event->pos().rx() > 406 && event->pos().rx() < 437
        && event->pos().ry() > 464 && event->pos().ry() < 495)
    {

        QPixmap pixmap(*volumeSoundPxmap);
        QMatrix rm;


        if (event->angleDelta().ry() > 0 && angleSnd < 100)
            angleSnd += 8;
        else if (event->angleDelta().ry() < 0 && angleSnd > -24)
            angleSnd -= 8;
        rm.rotate(angleSnd);

        pixmap = pixmap.transformed(rm);

        ui->volumeSound->setPixmap(QPixmap(pixmap.transformed(rm)));

        controller.changeVolume((angleSnd + 24) * 100 / 124);
    }
}

void R173::perest()
{
    static bool activate = false;

    this->setEnableToAll(activate);

    if (!activate)
    {
        prd_on();
    }
    else
    {
        prd_off();
    }

    activate = !activate;
}

void R173::call_on()
{
    this->ui->call->setStyleSheet("background-image: url(:/res/p-173m-call_on.jpg);");
}

void R173::call_off()
{
    this->ui->call->setStyleSheet("background-image: url(:/res/p-173m-call_off.jpg);");
}

void R173::on_button0_clicked()
{
    this->controller.ButtonPressed(R173Controller::B0);
}

void R173::on_button1_clicked()
{
    this->controller.ButtonPressed(R173Controller::B1);
}

void R173::on_button2_clicked()
{
    this->controller.ButtonPressed(R173Controller::B2);
}

void R173::on_button3_clicked()
{
    this->controller.ButtonPressed(R173Controller::B3);
}

void R173::on_button4_clicked()
{
    this->controller.ButtonPressed(R173Controller::B4);
}

void R173::on_button5_clicked()
{
    this->controller.ButtonPressed(R173Controller::B5);
}

void R173::on_button6_clicked()
{
    this->controller.ButtonPressed(R173Controller::B6);
}

void R173::on_button7_clicked()
{
    this->controller.ButtonPressed(R173Controller::B7);
}

void R173::on_button8_clicked()
{
    this->controller.ButtonPressed(R173Controller::B8);
}

void R173::on_button9_clicked()
{
    this->controller.ButtonPressed(R173Controller::B9);
}

void R173::on_buttonCls_clicked()
{
    this->controller.ButtonPressed(R173Controller::BCLS);
}

void R173::on_pomiechi_clicked()
{
    static bool switchPomiechi = false;
    if (!switchPomiechi)
    {
         this->ui->pomiechi->setStyleSheet("border-image: url(:/res/pomiechi_r.jpg);");
    }
    else
    {
        this->ui->pomiechi->setStyleSheet("border-image: url(:/res/pomiechi.jpg);");
    }
    switchPomiechi = !switchPomiechi;
    this->controller.ButtonPressed(R173Controller::BPOMIECHI);
}

void R173::on_powerLevel_clicked()
{
    static bool switchPowerLevel = false;
    if (!switchPowerLevel)
    {
        this->ui->powerLevel->setStyleSheet("border-image: url(:/res/powerLavel_r.jpg);");
    }
    else
    {
        this->ui->powerLevel->setStyleSheet("border-image: url(:/res/powerLavel.jpg);");
    }
    switchPowerLevel = !switchPowerLevel;
    this->controller.ButtonPressed(R173Controller::BPOWERLEVEL);
}

void R173::on_szum_clicked()
{
    static bool switchSzum = false;
    if (!switchSzum)
    {
         this->ui->szum->setStyleSheet("border-image: url(:/res/szum_r.jpg);");
    }
    else
    {
        this->ui->szum->setStyleSheet("border-image: url(:/new/prefix1/res/szum.jpg);");
    }
    switchSzum = !switchSzum;
    this->controller.ButtonPressed(R173Controller::BSZUM);
}

void R173::on_power_clicked()
{
    static bool switchPower = false;
    if (!switchPower)
    {
         this->ui->power->setStyleSheet("border-image: url(:/res/power_r.jpg);");
    }
    else
    {
        this->ui->power->setStyleSheet("border-image: url(:/new/prefix1/res/power.jpg);");
    }
    switchPower = !switchPower;
    this->controller.ButtonPressed(R173Controller::BPOWER);
}

void R173::on_tableSwitch_clicked()
{
    static bool switchTable = false;
    if (!switchTable)
    {
         this->ui->tableSwitch->setStyleSheet("border-image: url(:/res/table_r.jpg);");
    }
    else
    {
        this->ui->tableSwitch->setStyleSheet("border-image: url(:/res/table.jpg);");
    }
    switchTable = !switchTable;
    this->controller.ButtonPressed(R173Controller::BTABLE);
}

void R173::on_ton_pressed()
{
    this->ui->ton->setStyleSheet("border-image: url(:/res/p-173m-ton_pressed.jpg);");
    this->controller.ButtonPressed(R173Controller::BTON);
}

void R173::on_ton_released()
{
    this->ui->ton->setStyleSheet("border-image: url(:/res/p-173m-ton_released.jpg);");
    this->controller.buttonReleased(R173Controller::BTON);
}
