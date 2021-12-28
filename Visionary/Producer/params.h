//
// Created by amir sarusi on 25/12/2021.
//

#ifndef PRODUCER_PARAMS_H
#define PRODUCER_PARAMS_H
#define MAX_BUFFER_SIZE 2e24
#define LOCAL_HOST "127.0.0.1"
#define CONSUMER_PORT 2012
#define PRODUCER_PORT 2013
#define PACK_SIZE 4096 //udp pack size; note that OSX limits < 8100 bytes
#define ENCODE_QUALITY 80

typedef std::pair<std::pair<int,int>,std::vector<unsigned char>> MatSend;



#endif //PRODUCER_PARAMS_H
