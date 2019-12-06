#include "multicastnetworkcontroller.h"

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
    this->group.sin_addr.s_addr = inet_addr("224.4.4.44");
    this->group.sin_port = htons(DEFAULT_PORT);

    int flag;
    flag = bind(sock_fd, (SOCKADDR*)&s_addr, sizeof(s_addr));
    //if (flag == -1) { }

    u_int32 grpaddr;
    QByteArray ba = group_ip.toLocal8Bit();
    const char *c_group_ip = ba.data();
    inet_pton(AF_INET, c_group_ip, &(grpaddr));
    char *srcaddr_str = "127.0.0.1";
    u_int32 srcaddr;
    inet_pton(AF_INET, srcaddr_str, &srcaddr);
    u_int32  iaddr = srcaddr;
    this->join_source_group(sock_fd, grpaddr, srcaddr, iaddr);
    this->run();
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


void MulticastNetworkController::run() {
    auto audioOut = this->audioOutput->start();
    auto audioIn = this->audioInput->start();

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
            send(nothing);
            receive(nothing);
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
