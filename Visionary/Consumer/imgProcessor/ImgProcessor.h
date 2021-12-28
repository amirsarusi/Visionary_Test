//
// Created by amir sarusi on 25/12/2021.
//

#ifndef CONSUMER_IMGPROCESSOR_H
#define CONSUMER_IMGPROCESSOR_H

#include <opencv2/opencv.hpp>
#include "../udpListener/udpListener.h"
#include "../params.h"
#include "../blockingQueue/blockingQueue.h"

using namespace cv;

class imgProcessor {
public:
    imgProcessor(blockingQueue<std::pair<int,cv::Mat>>& qOut, blockingQueue<preProcessedImg>& qPri,volatile bool& shouldStop,int threadID);
    ~imgProcessor();
    void processImg();
private:
    void cleanup();

    blockingQueue<std::pair<int,cv::Mat>>& qOut;
    blockingQueue<preProcessedImg>& qPri;
    volatile bool& shouldStop;
    int threadID;
    int processor_count;
};


#endif //CONSUMER_IMGPROCESSOR_H
