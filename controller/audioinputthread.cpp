#include "controller/soundcontroller.h"
#include <QDebug>


SoundController::AudioInputThread::AudioInputThread(QAudioInput * audioInput, QAudioOutput * audioOutput)
{
    this->audioInput = audioInput;
    this->audioOutput = audioOutput;
}

void SoundController::AudioInputThread::run()
{
//    this->audioInput->setBufferSize(1024);
//    this->audioOutput->setBufferSize(1024);

    auto i = this->audioInput->start();
    auto j = this->audioOutput->start();

    char buffer[1024] = {0};

    while (true) {
        mutex.lock();

        //qDebug() << this->audioInput->bytesReady();

        if (pause_f)
        {
            this->audioInput->resume();
            this->audioOutput->resume();
            memset(buffer, 0, 1024);
            pause_f = false;

        }

        i->read(buffer, 1024);
        //this->msleep(25);
        //for (int i = 0; i < 1024; i++)
        //    buffer[i]-=(buffer[i]*2);
        j->write(buffer, 1024);

        mutex.unlock();
    }
}

void SoundController::AudioInputThread::pause()
{
    mutex.lock();

    this->audioInput->suspend();
    this->audioOutput->suspend();
}

void SoundController::AudioInputThread::resume()
{
    pause_f = true;
    mutex.unlock();
}
