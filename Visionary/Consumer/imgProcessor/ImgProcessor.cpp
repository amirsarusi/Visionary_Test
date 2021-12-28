//
// Created by amir sarusi on 27/12/2021.
//
#include "ImgProcessor.h"

imgProcessor::imgProcessor(blockingQueue<std::pair<int,cv::Mat>>& qOut, blockingQueue<preProcessedImg>& qPri,volatile bool& shouldStop,int threadID): qOut(qOut), qPri(qPri),shouldStop(shouldStop),threadID(threadID)
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
                auto pri = qPri.Take();
                Mat rawData = Mat(1,PACK_SIZE * pri.numOfPackets,CV_8UC1,pri.data);
                Mat frame = imdecode(rawData, IMREAD_COLOR);
                if(frame.data == 0) // failed to decode
                {
                    delete pri.data;
                    return;
                }
                std::cout << "thread number " << threadID << " processed image id: " << pri.id << std::endl;
                qOut.Put(std::make_pair(pri.id,frame));
                free(pri.data);

            }catch(Exception e)
            {
                std::cout << "thread number " << threadID << " has encountered an error while decoding an image " << std::endl;
                continue;
            }
        }
    }
    if(!qPri.empty())
    {
        cleanup();
    }
}

void imgProcessor::cleanup()
{
    do{
        try
        {
            auto pri = qPri.Take();
            Mat rawData = Mat(1,PACK_SIZE * pri.numOfPackets,CV_8UC1,pri.data);
            Mat frame = imdecode(rawData, IMREAD_COLOR);
            if(frame.data == 0) // failed to decode
            {
                delete pri.data;
                return;
            }
            std::cout << "thread number " << threadID << " processed image id: " << pri.id << std::endl;
            qOut.Put(std::make_pair(pri.id,frame));
            free(pri.data);

        }catch(Exception e)
        {
            std::cout << "thread number " << threadID << " has encountered an error while decoding an image " << std::endl;
            continue;
        }
    }while(!qPri.empty());
}
