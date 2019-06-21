#include "networkcontroller.h"
#include <QAudioDeviceInfo>
#include <QDebug>
#include <math.h>
#include <immintrin.h>

void NetworkController::run()
{

    auto audioOut = this->audioOutput->start();
    auto audioIn = this->audioInput->start();

    bool switched = false;
    bool call = false;

    qDebug("network thread is run");

    while(n_exit)
    {
        this->change_state.lock();

        if (this->is_config)
        {
            if (this->is_send)
            {
                if (this->is_call)
                {
                    if (call != true)
                    {
                        switched = true;
                    }
                    call = true;

                    client->send(callingSound);
                    client->recieve(callingSound);

                    audioOut->write(callingSound.audio_data, MESSAGE_SIZE);

                }
                else
                {
                    if (call != false)
                    {
                        switched = true;
                    }
                    call = false;
                    audioIn->read(sendSound.audio_data, MESSAGE_SIZE);

                    client->send(sendSound);
                    client->recieve(sendSound);


                    audioOut->write(sendSound.audio_data, MESSAGE_SIZE);
                }
            }
            else
            {
                memset(this->recieveSound.audio_data, '\0', MESSAGE_SIZE);
                client->send(this->recieveSound);
                client->recieve(this->recieveSound);


                prepareToAudioOutput(recieveSound);

                audioOut->write(recieveSound.audio_data, MESSAGE_SIZE);

                if (recieveSound.call)
                {
                    if (call == false)
                    {
                        switched = true;
                    }
                    call = true;
                    recieveSound.call = false;
                }
                else
                {
                    if (call == true)
                    {
                        switched = true;
                    }
                    call = false;
                }
            }
        }
        else
        {

            client->send(nothing);
            client->recieve(nothing);
        }

        this->change_state.unlock();

        if(switched)
        {
            switched = false;
            if (call)
            {
                emit set_call();
            }
            else
            {
                emit reset_call();
            }
        }
    }
}

Message NetworkController::getSzum()
{
    Message message;
    for (int i = 0; i <MESSAGE_SIZE; i ++)
        message.audio_data[i] = rand() % 100;

    return message;
}

void NetworkController::prepareToAudioOutput(Message &msg)
{
    Message szum = getSzum();

/*
    destiny = reinterpret_cast<__m128i*>(msg.audio_data);
    source = reinterpret_cast<__m128i*>(szum.audio_data);
    for (int i = 0; i < MESSAGE_SIZE / 16; i++)
    {
        destiny[i] += source[i];
    }
*/

    for (int i = 0; i < MESSAGE_SIZE; i++)
    {
        szum.audio_data[i] *= szum_level;
        msg.audio_data[i] += szum.audio_data[i];
    }
}

NetworkController::NetworkController(bool server, QString ip)
{
    if (server)
        this->server = new Server();
    else
        this->server = nullptr;


    nothing.call = false;
    nothing.sending = false;
    nothing.disconect = false;
    nothing.frequency = -1;

    sendSound.call = false;
    sendSound.sending = true;
    sendSound.disconect = false;

    callingSound.call = true;
    callingSound.sending = true;
    callingSound.disconect = false;
    for (int i = 0; i < MESSAGE_SIZE; i++)
        callingSound.audio_data[i] = char(50 * sin(i * MESSAGE_SIZE));

    recieveSound.call = false;
    recieveSound.sending = false;
    recieveSound.disconect = false;


    this->n_exit = true;

    QAudioDeviceInfo infoInput = QAudioDeviceInfo::defaultInputDevice();
    QAudioDeviceInfo infoOutput = QAudioDeviceInfo::defaultOutputDevice();

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(RATE_BYTES);
    format.setSampleType(RATE_BYTES == 8 ? QAudioFormat::UnSignedInt : QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    this->audioInput = new QAudioInput(infoInput, format);
    this->audioOutput = new QAudioOutput(infoOutput, format);


    if (server)
        this->client = new Client("127.0.0.1");
    else
        this->client = new Client(ip);

    this->is_call = false;
    this->is_send = false;
    this->is_config = false;

    this->szum_level = 2;
}

void NetworkController::config_send(int frequency)
{
    this->change_state.lock();

    this->is_config = true;
    this->is_send = true;

    sendSound.frequency = frequency;
    callingSound.frequency = frequency;

    this->change_state.unlock();
}

void NetworkController::config_listen(int frequency)
{
    this->change_state.lock();

    this->is_config = true;
    this->is_send = false;

    recieveSound.frequency = frequency;

    this->change_state.unlock();
}

void NetworkController::config_kill()
{
    this->change_state.lock();

    this->is_config = false;

    this->change_state.unlock();
}

void NetworkController::call_on()
{
    this->change_state.lock();

    this->is_call = true;

    this->change_state.unlock();
}

void NetworkController::call_off()
{
    this->change_state.lock();

    this->is_call = false;

    this->change_state.unlock();
}

void NetworkController::setVolume(char level)
{
    change_state.lock();
    this->audioOutput->setVolume(qreal(level) / 100);
    change_state.unlock();
}

void NetworkController::setSzumLevel(float level)
{
    change_state.lock();
    this->szum_level = level;
    change_state.unlock();
}

void NetworkController::setRecordVolume(char level)
{
    change_state.lock();
    this->audioInput->setVolume(qreal(level) / 100);
    change_state.unlock();
}

NetworkController::~NetworkController()
{
    if (server)
        delete server;
    delete client;

    delete audioInput;
    delete audioOutput;

    this->n_exit = false;
}
