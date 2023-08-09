//
// Created by 21553 on 2023/6/22.
//

#ifndef RW_CPPS_PACKET_H
#define RW_CPPS_PACKET_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
#include "winsock2.h"
// win64
#else
// win32
#endif
#elif __linux__
#include <sys/socket.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <map>
#include "PacketType.h"

#define PACKET_HEAD_SIZE (sizeof(uint32_t)*2)

using namespace std;
typedef string BytesArray;
typedef char Byte;

class Packet {
public:
    Packet();

    explicit Packet(PacketType packetType);
    explicit Packet(BytesArray *_bytesArray);
    explicit Packet(BytesArray _bytesArray);

    Packet(BytesArray _bytesArray,PacketType packetType);
    //写入数据，复制字节
    bool Write(const char *src,size_t _n);
    //准备发送Packet时,填充contentLength与packetType
    bool Prepare();

    
    //铁锈数据包类型
    PacketType packetType = PacketType::NOT_RESOLVED;
    /*  |0   1   2   3|4   5   6   7|8 9 10 11
        ContentLength |packetType   |Data
        ContentLength即除去前8个字节剩下的大小
     */
    uint32_t ContentLength = 0;

    BytesArray bytesArray;
};



#endif //RW_CPPS_PACKET_H
