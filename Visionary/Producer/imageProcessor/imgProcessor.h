//
// Created by amir sarusi on 27/12/2021.
//

#ifndef PRODUCER_IMGPROCESSOR_H
#define PRODUCER_IMGPROCESSOR_H


#include <vector>
#include <queue>
#include "../params.h"

class imgProcessor {
public:
    imgProcessor(  std::vector<std::string> imgPaths, std::queue<MatSend>& qSend,int thread_id);
    void processImages();
private:
    std::vector<std::string> imgPaths;
    std::queue<MatSend>& qSend;
    int thread_id;


};


#endif //PRODUCER_IMGPROCESSOR_H
