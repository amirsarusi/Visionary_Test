//
// Created by amir sarusi on 24/12/2021.
//

#include <iostream>
#include "udpListener.h"
#include "../imgProcessor/ImgProcessor.h"
#include <thread>

udpListener:: udpListener(std::string local_ip,std::string f_ip ,int local_port, int f_port ,bool& shouldStop,std::queue<std::pair<int,cv::Mat>>& qOut,std::vector<std::queue<preProcessedImg>>& vWorkersQ):
local_ip(local_ip),f_ip(f_ip),local_port(local_port),f_port(f_port),shouldStop(shouldStop),udp_socket(local_ip,local_port),qOut(qOut),vWorkersQ(vWorkersQ)
{

}

void udpListener::listen()
{
    auto size_rec = -1;
    const auto processor_count = std::thread::hardware_concurrency() / 2;
    auto workerShouldStop = false;
    auto iterationNum = 0;
    int headerBuff[2];
    int buffer[PACK_SIZE];
    preProcessedImg pri;
    while(true)
    {
        // when we send a message the first int that arrives are the number of packets the image was split to
        do {
            size_rec = udp_socket.recvFrom(headerBuff, 2 * sizeof(int), (std::string &) LOCAL_HOST, f_port);
        } while (size_rec > 2 * sizeof(int));
        auto id = ((int*) headerBuff)[0];
        auto total_pack = ((int*) headerBuff)[1];

        if(id == -1 && total_pack == -1)
        {
            shouldStop = true;
            break;
        }
        ///std::cout << "udpListener expecting image split into: " << total_pack << " packets" << std::endl;
        char* rawImageBuff = new char[PACK_SIZE * total_pack];
        for (int i = 0; i < total_pack; i++) {
            size_rec = udp_socket.recvFrom(buffer, PACK_SIZE, (std::string &) LOCAL_HOST, f_port);
            if (size_rec != PACK_SIZE) {
                std::cerr << "Received unexpected size pack:" << size_rec << std::endl;
                continue;
            }
            memcpy( &rawImageBuff[i * PACK_SIZE], buffer, PACK_SIZE);
        }

        pri.id = id;
        pri.numOfPackets = total_pack;
        pri.data = new char[total_pack*PACK_SIZE];
        memcpy(pri.data,rawImageBuff,total_pack*PACK_SIZE);
        vWorkersQ[iterationNum % processor_count].push(pri);
        free(rawImageBuff);
        iterationNum++;
    }
    shouldStop = true;
}

udpListener::~udpListener()
{
    udp_socket.disconnect();
    udp_socket.cleanUp();
}



