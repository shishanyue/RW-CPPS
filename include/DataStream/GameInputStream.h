//
// Created by 21553 on 2023/6/22.
//

#ifndef RW_CPPS_GAMEINPUTSTREAM_H
#define RW_CPPS_GAMEINPUTSTREAM_H

#include "AbstractDataStream.h"

class GameInputStream: public AbstractDataStream {
public:
    GameInputStream();
    explicit GameInputStream(Packet *packet);
    GameInputStream(Packet *packet, PacketType packetType);
    explicit GameInputStream(PacketType packetType);

    bool prepareNewPacketBuff();
    uint8_t readByte();
    uint32_t readInt();
    uint16_t readShort();
    string readString();
    string readStreamBytes();
    string readIfIsString();
    bool readBool();
    void skin(size_t _n);

private:
    uint32_t PacketIndex = 0;
    Byte *bytesF{};
};


#endif //RW_CPPS_GAMEINPUTSTREAM_H
