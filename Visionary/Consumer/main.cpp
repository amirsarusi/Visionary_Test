#include <iostream>
#include <opencv2/opencv.hpp>
#include "thread"
#include "params.h"
#include "udpListener/udpListener.h"
#include "imgProcessor/ImgProcessor.h"
#include <chrono>
#include "blockingQueue/blockingQueue.h"

using namespace std;
using namespace cv;



int main()
{
    auto shouldStop = false;
    auto workerShouldStop = false;
    auto qOutput = blockingQueue<std::pair<int,Mat>>();
    const auto processor_count = std::thread::hardware_concurrency() / 2;
    //const auto processor_count =  1;

    auto vWorkerQ = std::vector<blockingQueue<preProcessedImg>>(processor_count);
    auto vThreads = std::vector<std::thread>(processor_count);
    for(int i = 0; i < processor_count ; i++)
    {
        vThreads[i] = std::thread(&imgProcessor::processImg, imgProcessor(qOutput,vWorkerQ[i],workerShouldStop,i));
    }
    auto udp_rec = udpListener(LOCAL_HOST, LOCAL_HOST, CONSUMER_PORT, PRODUCER_PORT, shouldStop,qOutput,vWorkerQ,processor_count);
    auto tListener = std::thread(&udpListener::listen,&udp_rec);
    auto gotFirst = false;
    auto totalImages = 0;
    cv::namedWindow( "[img]", WINDOW_AUTOSIZE );

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while(!shouldStop)
    {
        auto maxID = 0;
        while(!qOutput.empty())
        {
            if(!gotFirst)
            {
                begin = std::chrono::steady_clock::now();

                gotFirst = true;
            }
            auto pImg = qOutput.Take();
            if(pImg.first > maxID)
            {
                imshow("[img]",pImg.second);
                cv::waitKey(10);
                maxID = pImg.first;
            }
            totalImages++;

        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    workerShouldStop = true;
    tListener.join();
    for(auto& t : vThreads)
    {
        t.join();
    }

    char n;
    std::cout << totalImages << " have been streamed and displayed for a total of " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count()
        << " [seconds] " << "for average fps of: " << (double)totalImages / std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[frame / second]" << std::endl;
    std::cout << "press any key then enter key to exit..." << std::endl;
    std::cin >> n;
    cv::destroyAllWindows();
    return 0;
}