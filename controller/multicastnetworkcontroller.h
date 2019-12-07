#ifndef MULTICASTNETWORKCONTROLLER_H
#define MULTICASTNETWORKCONTROLLER_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <mswsock.h>
#define u_int32 UINT32
#include <winsock2

#else //let it be unix
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define SOCKADDR sockaddr
#define u_int32 uint32_t
#endif

#include "model/client.h"
#include "model/server.h"
#include "model/message.h"
#include <controller/networkcontroller/abstractnetworkcontroller.h>
#include <QThread>
#include <QAudioInput>
#include <QAudioOutput>
#include "instruments/socket.h"

#define GROUP_IP "14.88.228.0"
#define DEFAULT_PORT 56666

class MulticastNetworkController: public AbstractNetworkController
{
private:
    /*multicast stuff*/
    SOCKET sock_fd;
    sockaddr_in group;
    /**/

    bool n_exit;
    QMutex change_state;

    Message nothing;
    Message sendingSound;
    Message callingSound;
    Message receivedSound;

    bool is_transmitting;
    bool is_call;
    bool is_configured;
    QAudioInput * audioInput;
    QAudioOutput * audioOutput;

    float szum_level;

    /*MSDN example*/
    int join_source_group(/*socket*/int sd, u_int32 grpaddr,
       u_int32 srcaddr, u_int32 iaddr)
    {
       struct ip_mreq_source imr;

       imr.imr_multiaddr.s_addr  = grpaddr;
       imr.imr_sourceaddr.s_addr = srcaddr;
       imr.imr_interface.s_addr  = iaddr;
       return setsockopt(sd, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char *) &imr, sizeof(imr));
    }

    int leave_source_group(/*socket*/int sd, u_int32 grpaddr,
       u_int32 srcaddr, u_int32 iaddr)
    {
       struct ip_mreq_source imr;

       imr.imr_multiaddr.s_addr  = grpaddr;
       imr.imr_sourceaddr.s_addr = srcaddr;
       imr.imr_interface.s_addr  = iaddr;
       return setsockopt(sd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, (char *) &imr, sizeof(imr));
       /**/
    }
    void send(Message &msg) {
        char *data = reinterpret_cast<char*>(&msg);
        sendto(this->sock_fd, data, sizeof(msg), 0, (const sockaddr*)&group, sizeof(group));
    }
    void receive(Message &msg) {
        char *data = reinterpret_cast<char*>(&msg);
        int ret_size = recv(this->sock_fd, data, sizeof(msg), 0);

    }
    Message getSzum() {
        Message message;
        for (int i = 0; i <MESSAGE_SIZE; i ++)
            message.audio_data[i] = rand() % 100;

        return message;
    }
    void prepareForAudioOutput(Message &msg);
    void run() override;
public:
    void config_send(int frequency) override;
    void config_listen(int frequecy) override;
    void config_kill() override;

    void call_on() override;
    void call_off() override;
    void setVolume(char level) override;
    void setSzumLevel(float level) override;
    void setRecordVolume(char level) override;
    MulticastNetworkController();
    MulticastNetworkController(bool server, QString ip = "127.0.0.1", QString group_ip = GROUP_IP);
};

#endif // MULTICASTNETWORKCONTROLLER_H
