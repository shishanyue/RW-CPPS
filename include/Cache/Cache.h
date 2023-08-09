//
// Created by 21553 on 2023/7/31.
//

#ifndef RW_CPPS_CACHE_H
#define RW_CPPS_CACHE_H

#include <map>
#include <cstring>

#include "Packet/Packet.h"

using namespace std;
class Cache{
public:
    Cache();
    map<const string,Packet> packetCache;
};


#endif //RW_CPPS_CACHE_H
