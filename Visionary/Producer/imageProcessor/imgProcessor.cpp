//
// Created by amir sarusi on 27/12/2021.
//

#include "imgProcessor.h"
#include <opencv2/opencv.hpp>
#include "../params.h"

imgProcessor::imgProcessor(std::vector<std::string> imgPaths, std::queue<MatSend>& qSend,int thread_id,int numCpu): imgPaths(imgPaths),qSend(qSend), thread_id(thread_id),numCpu(numCpu)
{
    std::cout << "imgProcessor " << thread_id << " has been instantiated" << std::endl;
}

void imgProcessor::processImages()
{
    cv::Mat img;
    std::vector<uchar> encodedImg;
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(ENCODE_QUALITY);
    auto numOfIterations = 0;
    for(auto& path : imgPaths)
    {
        try
        {
            img = cv::imread(path);
            cv::imencode(".jpg", img, encodedImg, compression_params);
            auto total_pack = 1 + (encodedImg.size() - 1) / PACK_SIZE;
            auto pEncodedImg = std::make_pair(std::make_pair(thread_id + numCpu * numOfIterations,total_pack),encodedImg);
            qSend.push(pEncodedImg);

        }catch(cv::Exception e)
        {
            std::cout << "thread " << thread_id << " has encountered a problem encoding an image" << std::endl;
            continue;
        }
        numOfIterations++;
    }
    std::cout << "thread " << thread_id << " has finished processing" << std::endl;
}
