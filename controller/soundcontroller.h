#ifndef SOUNDCONTROLLER_H
#define SOUNDCONTROLLER_H

#include <QThread>
#include <QMutex>
#include <QAudioInput>
#include <QAudioOutput>


class SoundController
{
    QAudioInput * audioInput;
    QAudioOutput * audioOutput;

    class AudioInputThread : public QThread
    {
        //Q_OBJECT


        QMutex mutex;

        QAudioInput * audioInput;
        QAudioOutput * audioOutput;
        bool pause_f;

    public:

        AudioInputThread(QAudioInput * audioInput, QAudioOutput * audioOutput);

        void run() override;
        void pause();
        void resume();
    } * audioInputThread;

    class AudioOutputThread : public QThread
    {
        //Q_OBJECT
        QMutex mutex;
    public:
        void run() override;
        void pause();
    } * audioOutputThread;




public:
    SoundController();

    void startRecord();
    void stopRecord();

    void startPlay();
    void stopPlay();
};

#endif // SOUNDCONTROLLER_H
