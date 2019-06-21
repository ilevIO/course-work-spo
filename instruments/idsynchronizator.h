#ifndef IDSYNCHRONIZATOR_H
#define IDSYNCHRONIZATOR_H
#include <QThread>
#include <instruments/broadcastsocket.h>
#include <deque>
using std::deque;

struct idCounterMessage {
    int count;
    int ids[256];
    bool request;
    bool validFlag;
    bool pulse;
    int senderId;
};

class IdSynchronizator : QThread
{
    BroadcastSocket controlSocket, signalSocket;
    int id;

    friend class IdPulsator;
    class IdPulsator {
        deque<int> ids;
    public:
        deque<int>& getAvailableIds() const;

    };

public:
    IdSynchronizator();
    void run() override;

signals:
    void synchro();
};

#endif // IDSYNCHRONIZATOR_H
