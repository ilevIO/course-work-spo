#include <view/r173.h>
#include <view/mainmenu.h>
#include <QApplication>
#include <controller/broadcastnetworkcontroller.h>

#include "view/r123.h"
#include <QThread>
#include <QDebug>
#include <thread>
#include <chrono>
#include "instruments/webCounter.h"
#include <math.h>
using namespace std;

int main(int argc, char *argv[])
{
    Socket::init();

    QApplication a(argc, argv);

    MainMenu w;
    w.show();

    int ret_val = a.exec();

    Socket::clear();

    return ret_val;
}
