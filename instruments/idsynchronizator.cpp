#include "idsynchronizator.h"

IdSynchronizator::IdSynchronizator()
{
    this->controlSocket.bind(nullptr, 3000);
    this->signalSocket.bind(nullptr, 3001);
    this->start(QThread::LowestPriority);
}

void IdSynchronizator::run()
{

}


