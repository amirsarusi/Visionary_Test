#include <iostream>
#include <opencv2/opencv.hpp>
#include "thread"
#include "params.h"
#include "udpListener/udpListener.h"
#include "imgProcessor/ImgProcessor.h"

using namespace std;
using namespace cv;




int main()
{
    auto shouldStop = false;
    auto workerShouldStop = false;
    auto preProcessedQueue = std::queue<preProcessedImg>();
    auto qOutput = std::queue<std::pair<int,Mat>>();
    const auto processor_count = std::thread::hardware_concurrency() / 2;
    auto vWorkerQ = std::vector<std::queue<preProcessedImg>>(processor_count);
    auto vThreads = std::vector<std::thread>(processor_count);
    for(int i = 0; i < processor_count ; i++)
    {
        vWorkerQ[i] = std::queue<preProcessedImg>();
        vThreads[i] = std::thread(&imgProcessor::processImg, imgProcessor(qOutput,vWorkerQ[i],workerShouldStop,i));
    }
    auto udp_rec = udpListener(LOCAL_HOST, LOCAL_HOST, CONSUMER_PORT, PRODUCER_PORT, shouldStop,qOutput,vWorkerQ);
    auto tListener = std::thread(&udpListener::listen,&udp_rec);
    auto gotFirst = false;
    cv::namedWindow( "[img]", WINDOW_AUTOSIZE );
    while(!shouldStop)
    {
        auto maxID = 0;
        while(!qOutput.empty())
        {
            if(!gotFirst)
            {
                tBegin = clock();
                gotFirst = true;
            }
            auto pImg = qOutput.front();
            qOutput.pop();
            if(pImg.first > maxID)
            {
                imshow("[img]",pImg.second);
                cv::waitKey(10);
                maxID = pImg.first;
            }

        }
    }
    workerShouldStop = true;
    for(auto& t : vThreads)
    {
        t.join();
    }
    std::destroy(vWorkerQ.begin(), vWorkerQ.end());
    std::destroy(vThreads.begin(), vThreads.end());
    std::cout << "press any key to continue..." << std::endl;
    while(true){}
    return 0;
}