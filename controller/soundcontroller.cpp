#include "soundcontroller.h"

SoundController::SoundController()
{
    QAudioDeviceInfo infoInput = QAudioDeviceInfo::defaultInputDevice();
    QAudioDeviceInfo infoOutput = QAudioDeviceInfo::defaultOutputDevice();

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    this->audioInput = new QAudioInput(infoInput, format);
    this->audioOutput = new QAudioOutput(infoOutput, format);

    this->audioInputThread = new AudioInputThread(audioInput, audioOutput);
    this->audioInputThread->pause();
    this->audioInputThread->start();
}



void SoundController::startPlay()
{
    this->audioInputThread->resume();
}

void SoundController::stopPlay()
{
    this->audioInputThread->pause();
}

void SoundController::startRecord()
{

}

void SoundController::stopRecord()
{

}

