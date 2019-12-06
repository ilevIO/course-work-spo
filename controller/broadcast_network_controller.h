#ifndef BROADCAST_NETWORK_CONTROLLER_H
#define BROADCAST_NETWORK_CONTROLLER_H
/*
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#define NULL nullptr
typedef int SOCKET;
#endif
#include <vector>
#include "networkcontroller.h"

class BroadcastNetworkController: public NetworkController {
private:
    int sock;
public:
    std::vector<Client*> clients;
void run() override {
    auto audioOut = this->audioOutput->start();
    auto audioIn = this->audioInput->start();

    bool switched = false;
    bool call = false;
    bool n_exit = true;
    while(n_exit) {

    }
    std::vector<sockaddr_in> lookupConnections() {
            setenv("LANG","C",1);
            FILE *fp = popen("ifconfig", "r");
            char *line_ptr = NULL;
            size_t bytes_read = 0;
            std::vector<sockaddr_in> connections;
            int count = 0;
            while (getline(&line_ptr, &bytes_read, fp)) {
                char *ipaddr_line = NULL;
                if ((ipaddr_line = strstr(line_ptr, "inet"))) {
                    ipaddr_line += 4;
                    if ((ipaddr_line = strchr(ipaddr_line, ' '))) {
                        ++ipaddr_line;
                        char *ipaddr = (char*)malloc(strlen(ipaddr_line));
                        int i = 0;
                        int delim_count = 0;
                        for (i = 0; ipaddr_line[i] != ' '; i++) {
                            ipaddr[i] = ipaddr_line[i];
                            if (ipaddr[i] == '.') {
                                delim_count ++;
                            }
                        }
                        ipaddr[i] =  '\0';
                        if (delim_count == 3) {
                            sockaddr_in sock_addr;
                            inet_pton(AF_INET, ipaddr, &sock_addr);
                            count++;
                            printf("..%s\n", ipaddr);
                            sock_addr.sin_addr.s_addr = inet_addr(ipaddr);
                            connections.push_back(sock_addr);
                        }
                    }
                }
            }
            return connections;
    }
    BroadcastNetworkController(): NetworkController(true) {
        std::vector<sockaddr_in> ips;
        ips = lookupConnections();
        for (int i = 0; i < ips.size(); i++) {
            QString ipstr = QString(inet_ntop());

            Client *newClient = new Client(ipstr);
        }
    }
}
};
*/
#endif // BROADCAST_NETWORK_CONTROLLER_H
