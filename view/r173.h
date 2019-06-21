#ifndef R173_H
#define R173_H

#include <QMainWindow>
#include <controller/R173Controller.h>
#include "controller/statecontroller.h"
#include "controller/broadcastnetworkcontroller.h"
#include <QMouseEvent>

namespace Ui {
class R173;
}

class R173 : public QMainWindow
{
    Q_OBJECT

    friend class R173Controller;

    R173Controller controller;

    bool pomiechi_reversed = false;
    bool szum_reversed = false;
    bool power_reversed = false;

    bool handle_key_events = true;

    virtual void keyPressEvent(QKeyEvent *ev) override;
    virtual void keyReleaseEvent(QKeyEvent *ev) override;

    void setEnableToAll(bool anable);

    void mousePressEvent(QMouseEvent * event) override;

    void antenna_is_pressed();
    void zeziemlinije_is_pressed();
    void telefon_is_pressed();
    void pitanije_is_pressed();
    void wheelEvent(QWheelEvent * event) override;
public:

    explicit R173(QString IP, bool is_server, AbstractNetworkController * controller);

    ~R173();

    void set_table(StateController::TableInfo info);

    void call_on();
    void call_off();

    void prd_on();    
    void prd_off();



public slots:

    void perest();

private slots:
    void on_button0_clicked();

    void on_button1_clicked();

    void on_button2_clicked();

    void on_button3_clicked();

    void on_button4_clicked();

    void on_button5_clicked();

    void on_button6_clicked();

    void on_button7_clicked();

    void on_button8_clicked();

    void on_button9_clicked();

    void on_buttonCls_clicked();

    void on_pomiechi_clicked();

    void on_powerLevel_clicked();

    void on_szum_clicked();

    void on_power_clicked();

    void on_tableSwitch_clicked();

    void on_ton_pressed();

    void on_ton_released();



private:
    Ui::R173 *ui;
};

#endif // R173_H
