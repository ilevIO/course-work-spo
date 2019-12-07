#include "multicastnetworkcontroller.h"
#include <QDebug>
#include <math.h>
MulticastNetworkController::MulticastNetworkController(): AbstractNetworkController()
{

}

MulticastNetworkController::MulticastNetworkController(bool server, QString ip, QString group_ip): AbstractNetworkController() {
    this->sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    s_addr.sin_port = htons(DEFAULT_PORT);

    this->group.sin_family = AF_INET;
    this->group.sin_addr.s_addr = inet_addr(GROUP_IP);
    this->group.sin_port = htons(DEFAULT_PORT);
    qDebug() << "MulticastNetworkController::0\n";
    int flag;
    flag = bind(sock_fd, (SOCKADDR*)&s_addr, sizeof(s_addr));
    qDebug() << "MulticastNetworkController::1\n";
    //if (flag == -1) { }

    u_int32 grpaddr;
    QByteArray ba = group_ip.toLocal8Bit();
    const char *c_group_ip = ba.data();
    inet_pton(AF_INET, c_group_ip, &(grpaddr));
    const char *srcaddr_str = "127.0.0.1";
    u_int32 srcaddr;
    inet_pton(AF_INET, srcaddr_str, &srcaddr);
    u_int32  iaddr = srcaddr;
    qDebug() << "MulticastNetworkController::2\n";
    this->join_source_group(sock_fd, grpaddr, srcaddr, iaddr);
    qDebug() << "MulticastNetworkController::3\n";
    this->commonInit();
    this->run();
    qDebug() << "MulticastNetworkController::4\n";
}

void MulticastNetworkController::prepareForAudioOutput(Message &msg)
{
    //what does it  serve for again?
    Message szum = getSzum();
    for (int i = 0; i < MESSAGE_SIZE; i++)
    {
        szum.audio_data[i] *= szum_level;
        msg.audio_data[i] += szum.audio_data[i];
    }
}

void MulticastNetworkController::commonInit() {

    this->nothing.call = false;
    nothing.sending = false;
    nothing.disconect = false;
    nothing.frequency = -1;

    this->sendingSound.call = false;
    sendingSound.sending = true;
    sendingSound.disconect = false;

    callingSound.call = true;
    callingSound.sending = true;
    callingSound.disconect = false;
    for (int i = 0; i < MESSAGE_SIZE; i++)
        callingSound.audio_data[i] = char(50 * sin(i * MESSAGE_SIZE));

    receivedSound.call = false;
    receivedSound.sending = false;
    receivedSound.disconect = false;


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

    this->is_call = false;
    this->is_transmitting = false;
    this->is_configured = false;

    this->szum_level = 2;
}
void MulticastNetworkController::run() {
    qDebug() << "MulticastNetworkController::run::-1\n";
    auto audioOut = this->audioOutput->start();
    qDebug() << "MulticastNetworkController::run::-0.5\n";
    auto audioIn = this->audioInput->start();
    qDebug() << "MulticastNetworkController::run::0\n";

    bool switched = false;
    bool call = false;

    qDebug("multicast network thread is run");
    bool n_exit = false;
    while(n_exit) {
        this->change_state.lock();

        if (this->is_configured)
        {
            if (this->is_transmitting)
            {
                if (this->is_call)
                {
                    if (call != true)
                    {
                        switched = true;
                    }
                    call = true;
                    send(callingSound);
                    receive(callingSound);

                    //client->send(callingSound);
                    //client->recieve(callingSound);

                    audioOut->write(callingSound.audio_data, MESSAGE_SIZE);

                }
                else
                {
                    if (call != false)
                    {
                        switched = true;
                    }
                    call = false;
                    audioIn->read(sendingSound.audio_data, MESSAGE_SIZE);

                    //client->send(sendingSound);
                    //client->recieve(sendingSound);
                    send(callingSound);
                    receive(callingSound);

                    audioOut->write(sendingSound.audio_data, MESSAGE_SIZE);
                }
            }
            else
            {
                memset(this->receivedSound.audio_data, '\0', MESSAGE_SIZE);
                send(receivedSound);
                receive(receivedSound);
                //client->send(this->recievedSound);
                //client->recieve(this->recievedSound);


                prepareForAudioOutput(receivedSound);

                audioOut->write(receivedSound.audio_data, MESSAGE_SIZE);

                if (receivedSound.call)
                {
                    if (call == false)
                    {
                        switched = true;
                    }
                    call = true;
                    receivedSound.call = false;
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
            qDebug() << "MulticastNetworkController::run::1\n";
            usleep(500);
            qDebug() << "MulticastNetworkController::run::2\n";
            send(nothing);
            qDebug() << "MulticastNetworkController::run::3\n";
            receive(nothing);
            qDebug() << "MulticastNetworkController::run::4\n";
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


void MulticastNetworkController::config_send(int frequency)
{
    this->change_state.lock();

    this->is_configured = true;
    this->is_transmitting = true;

    sendingSound.frequency = frequency;
    callingSound.frequency = frequency;

    this->change_state.unlock();
}

void MulticastNetworkController::config_listen(int frequency)
{
    this->change_state.lock();

    this->is_configured = true;
    this->is_transmitting = false;

    receivedSound.frequency = frequency;

    this->change_state.unlock();
}

void MulticastNetworkController::config_kill()
{
    this->change_state.lock();

    this->is_configured = false;

    this->change_state.unlock();
}

void MulticastNetworkController::call_on()
{
    this->change_state.lock();

    this->is_call = true;

    this->change_state.unlock();
}

void MulticastNetworkController::call_off()
{
    this->change_state.lock();

    this->is_call = false;

    this->change_state.unlock();
}

void MulticastNetworkController::setVolume(char level)
{
    change_state.lock();
    this->audioOutput->setVolume(qreal(level) / 100);
    change_state.unlock();
}

void MulticastNetworkController::setSzumLevel(float level)
{
    change_state.lock();
    this->szum_level = level;
    change_state.unlock();
}

void MulticastNetworkController::setRecordVolume(char level)
{
    change_state.lock();
    this->audioInput->setVolume(qreal(level) / 100);
    change_state.unlock();
}

