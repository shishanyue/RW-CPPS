//
// Created by 21553 on 2023/6/22.
//

#ifndef RW_CPPS_GAMEOUTPUTSTREAM_H
#define RW_CPPS_GAMEOUTPUTSTREAM_H

#include "AbstractDataStream.h"
#include "Packet/Packet.h"

class GameOutputStream: public AbstractDataStream {
public:
    GameOutputStream(const Packet *packet,const PacketType packetType);
    explicit GameOutputStream(const Packet *packet);
    explicit GameOutputStream(PacketType packetType);

    void writeByte(const uint8_t value);
    void writeBytes(const void *src,size_t _n);
    void writeInt(const uint32_t value);
    void writeShort(const uint16_t value);
    void writeString(const string *value);
    void writeString(const string& value);
    void writeString(const char *value);
    void writeIsString(const string *value);
    void writeIsString(const string& value);
    void writeIsString(const char *value);
    void writeBool(const bool value);
};


#endif //RW_CPPS_GAMEOUTPUTSTREAM_H
