//
// Created by amir sarusi on 24/12/2021.
//

#ifndef CONSUMER_UDPLISTENER_H
#define CONSUMER_UDPLISTENER_H

#include "../udpSocket/udpSocket.h"
#include <queue>
#include "../params.h"
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <opencv2/opencv.hpp>
#include "../blockingQueue/blockingQueue.h"

typedef struct {
    int id;
    int numOfPackets;
    char* data;
}preProcessedImg;

class udpListener
{
public:
    udpListener(std::string local_ip,std::string f_ip ,int local_port, int f_port ,bool& shouldStop, blockingQueue<std::pair<int,cv::Mat>>& qOut,std::vector<blockingQueue<preProcessedImg>>& vWorkersQ,int processor_count);
    ~udpListener();
    void listen();
private:
    std::string local_ip;
    std::string f_ip;
    unsigned short local_port;
    unsigned short f_port;
    volatile bool& shouldStop;
    UDPSocket udp_socket;
    blockingQueue<std::pair<int,cv::Mat>>& qOut;
    std::vector<blockingQueue<preProcessedImg>>& vWorkersQ;
    int processor_count;
};




#endif //CONSUMER_UDPLISTENER_H
