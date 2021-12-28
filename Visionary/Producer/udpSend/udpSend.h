//
// Created by amir sarusi on 25/12/2021.
//

#ifndef PRODUCER_UDPSEND_H
#define PRODUCER_UDPSEND_H

#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <thread>
#include "../params.h"
#include "../udpSocket/udpSocket.h"
#include <queue>

class udpSend {
public:
    udpSend(std::string local_ip,std::string f_ip ,int local_port, int f_port, int numOfImages, std::queue<MatSend>& qSend);
    ~udpSend();
    void send();
private:
    std::string local_ip;
    std::string f_ip;
    unsigned short local_port;
    unsigned short f_port;
    int numOfImages;
    std::queue<MatSend>& qSend;
    UDPSocket udp_socket;
};


#endif //PRODUCER_UDPSEND_H
