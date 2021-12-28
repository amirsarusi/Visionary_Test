//
// Created by amir sarusi on 25/12/2021.
//

#include "udpSend.h"

udpSend::udpSend(std::string local_ip,std::string f_ip ,int local_port, int f_port, int numOfImages, std::queue<MatSend>& qSend):
local_ip(local_ip),local_port(local_port),f_port(f_port),numOfImages(numOfImages),qSend(qSend),udp_socket(local_ip,local_port)
{

}

udpSend::~udpSend()
{
    std::cout << "udpSend has been instantiated" << std::endl;
}

void udpSend::send()
{
    auto tBegin = clock();
    auto totalPacks = 0;
    while(numOfImages > 0)
    {
        if(!qSend.empty())
        {
            auto eSend = qSend.front();
            int ibuf[2];
            ibuf[0] = eSend.first.first;
            ibuf[1] = eSend.first.second;
            udp_socket.sendTo(ibuf, 2*sizeof(int), local_ip, f_port);

            for (int i = 0; i < eSend.first.second; i++)
            {
                udp_socket.sendTo(&eSend.second[i * PACK_SIZE], PACK_SIZE, local_ip, f_port);
                totalPacks++;
            }
            std::cout << "udpSend has sent image #" << eSend.first.first << std::endl;
            qSend.pop();
            numOfImages--;
        }
    }
    auto tEnd = clock();
    auto duration = (tEnd - tBegin) / CLOCKS_PER_SEC;
    std::cout << "Total sending time:" << duration << " \tkbps:" << (PACK_SIZE * totalPacks / duration / 1024 * 8) << std::endl;

    int ibuf[2];
    ibuf[0] = -1;
    ibuf[1] = -1;
    udp_socket.sendTo(ibuf, 2*sizeof(int), local_ip, f_port);
}