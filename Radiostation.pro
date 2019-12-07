#-------------------------------------------------
#
# Project created by QtCreator 2019-03-03T12:58:53
#
#-------------------------------------------------

QT       += core gui \
        multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Radiostation
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

win32 {
LIBS += -lws2_32
}
SOURCES += \
    controller/multicastnetworkcontroller.cpp \
    controller/r123bpstatecontroller.cpp \
    controller/r123rststatecontroller.cpp \
    controller/r123statecontroller.cpp \
    controller/r123vsuastatecontroller.cpp \
    instruments/idsynchronizator.cpp \
        main.cpp \
    view/initialmenu.cpp \
    view/r173.cpp \
    instruments/socket.cpp \
    controller/R173Controller.cpp \
    model/client.cpp \
    model/server.cpp \
    controller/statecontroller.cpp \
    controller/networkcontroller.cpp \
    view/mainmenu.cpp \
    instruments/broadcastsocket.cpp \
    model/broadcastclient.cpp \
    instruments/webcounter.cpp \
    view/r159.cpp \
    view/r123.cpp \
    controller/r123controller.cpp

HEADERS += \
    controller/broadcast_network_controller.h \
    controller/multicastnetworkcontroller.h \
    controller/networkcontroller/abstractnetworkcontroller.h \
    controller/r123bpstatecontroller.h \
    controller/r123rststatecontroller.h \
    controller/r123statecontroller.h \
    controller/r123vsuastatecontroller.h \
    instruments/idsynchronizator.h \
    view/initialmenu.h \
    view/r173.h \
    instruments/socket.h \
    controller/R173Controller.h \
    model/client.h \
    model/server.h \
    model/message.h \
    controller/statecontroller.h \
    controller/networkcontroller.h \
    view/mainmenu.h \
    instruments/broadcastsocket.h \
    model/broadcastclient.h \
    instruments/webCounter.h \
    view/r159.h \
    view/r123.h \
    controller/r123controller.h

FORMS += \
    view/initialmenu.ui \
    view/r173.ui \
    view/mainmenu.ui \
    view/r159.ui \
    view/r123.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    view/r173.qrc \
    view/r159.qrc \
    view/r123.qrc


DISTFILES += \
    view/res/p173-nlo.jpg \
    view/res/p173-nlo.jpg
