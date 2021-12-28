#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <thread>
#include "imageProcessor/imgProcessor.h"
#include "params.h"
#include "udpSend/udpSend.h"

std::vector<std::string> getAllPaths(const std::string& path);

int main(int argc, char *argv[] ) {
    std::cout << std::filesystem::current_path() << std::endl;
    //std::string ImagePath = argv[1];
    std::string ImagePath = "/Images";
    std::string fullPath = std::filesystem::current_path().string() + ImagePath;
    //std::cout << "full path is: " << fullPath;
    std::vector<std::string> vIpath = getAllPaths(fullPath);
    std::sort(vIpath.begin(),vIpath.end());
    auto numCpu = std::thread::hardware_concurrency() / 2 ;
    auto vThread = std::vector<std::thread>(numCpu);
    auto vThreadPaths = std::vector<std::vector<std::string>>(numCpu);
    auto qOutSend = std::queue<MatSend>();
    auto sendUDP = udpSend(LOCAL_HOST,LOCAL_HOST,PRODUCER_PORT,CONSUMER_PORT,vIpath.size(),qOutSend);
    auto tSend = std::thread(&udpSend::send,&sendUDP);

    for(int i = 0 ; i < vIpath.size() ; i ++)
    {
        vThreadPaths[i % numCpu].push_back(vIpath[i]);
    }

    for(int i = 0; i < numCpu ; i++)
    {
        vThread[i] = std::thread(&imgProcessor::processImages, imgProcessor(vThreadPaths[i],qOutSend,i + 1));
    }

    for(int i = 0 ; i < numCpu ; i++)
    {
        vThread[i].join();
    }
    tSend.join();

    return 0;
}

std::vector<std::string> getAllPaths(const std::string& path)
{
    std::vector<std::string> vImages;
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        vImages.push_back(entry.path());
    }
    return vImages;
}

