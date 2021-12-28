//
// Created by amir sarusi on 25/12/2021.
//

#ifndef CONSUMER_IMGPROCESSOR_H
#define CONSUMER_IMGPROCESSOR_H

#include <opencv2/opencv.hpp>
#include "../udpListener/udpListener.h"
#include "../params.h"
using namespace cv;

class imgProcessor {
public:
    imgProcessor(std::queue<std::pair<int,cv::Mat>>& qOut, std::queue<preProcessedImg>& qPri,volatile bool& shouldStop,int threadID);
    ~imgProcessor();
    void processImg();
private:
    void cleanup();

    std::queue<std::pair<int,cv::Mat>>& qOut;
    std::queue<preProcessedImg>& qPri;
    volatile bool& shouldStop;
    int threadID;
};


#endif //CONSUMER_IMGPROCESSOR_H
