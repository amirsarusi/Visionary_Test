//
// Created by amir sarusi on 27/12/2021.
//
#include "ImgProcessor.h"

imgProcessor::imgProcessor(std::queue<std::pair<int,cv::Mat>>& qOut, std::queue<preProcessedImg>& qPri,volatile bool& shouldStop,int threadID): qOut(qOut), qPri(qPri),shouldStop(shouldStop),threadID(threadID)
{

}
imgProcessor::~imgProcessor()
{

}

void imgProcessor::processImg()
{
    while(!shouldStop)
    {
        if(!qPri.empty())
        {
            try
            {
                auto pri = qPri.front();
                qPri.pop();
                Mat rawData = Mat(1,PACK_SIZE * pri.numOfPackets,CV_8UC1,pri.data);
                Mat frame = imdecode(rawData, IMREAD_COLOR);
                if(frame.data == 0) // failed to decode
                {
                    delete pri.data;
                    return;
                }
                std::cout << "thread number " << threadID << " processed image id: " << pri.id << std::endl;
                qOut.push(std::make_pair(pri.id,frame));
                free(pri.data);

            }catch(Exception e)
            {
                std::cout << "thread number " << threadID << " has encountered an error while decoding an image " << std::endl;
                continue;
            }
        }
    }

}