//
// Created by 21553 on 2023/6/22.
//

#include "DataStream/GameInputStream.h"


GameInputStream::GameInputStream(Packet *packet, PacketType packetType){
    this->packet = *packet;
    this->packet.packetType = packetType;
}

GameInputStream::GameInputStream(Packet *packet){
    this->packet = *packet;
}

GameInputStream::GameInputStream(PacketType packetType) {
    this->packet.packetType = packetType;
}

GameInputStream::GameInputStream() {
    this->packet.packetType = PacketType::NOT_RESOLVED;
};



uint8_t GameInputStream::readByte() {
    this->bytesF = this->packet.bytesArray.data();
    this->PacketIndex += sizeof(uint8_t);
    return *reinterpret_cast<uint8_t *>(bytesF+PacketIndex - sizeof(uint8_t));
}

uint32_t GameInputStream::readInt() {
    this->bytesF = this->packet.bytesArray.data();
    this->PacketIndex += sizeof(uint32_t);
    return htonl(*reinterpret_cast<uint32_t *>(bytesF+PacketIndex-sizeof(uint32_t)));
}

uint16_t GameInputStream::readShort() {
    this->bytesF = this->packet.bytesArray.data();
    this->PacketIndex += sizeof(uint16_t);
    return ntohs(*reinterpret_cast<uint16_t *>(bytesF+PacketIndex-sizeof(uint16_t)));
}

string GameInputStream::readString() {
    this->bytesF = this->packet.bytesArray.data();
    uint16_t strLength = readShort();
    string str(bytesF+PacketIndex,0,strLength);
    this->PacketIndex += strLength;
    return str;
}

bool GameInputStream::readBool() {
    this->bytesF = this->packet.bytesArray.data();
    this->PacketIndex += sizeof(bool);
    return *reinterpret_cast<bool *>(bytesF+PacketIndex-sizeof(bool));
}


bool GameInputStream::prepareNewPacketBuff() {
    this->packet.bytesArray.clear();
    this->packet.ContentLength=0;
    this->PacketIndex=0;
    return true;
}

void GameInputStream::skin(size_t _n) {
    this->PacketIndex += _n;
}


string GameInputStream::readIfIsString() {
    return readBool()?readString():"";
}

string GameInputStream::readStreamBytes() {
    this->bytesF = this->packet.bytesArray.data();
    uint32_t _n = readInt();
    string str(bytesF+PacketIndex,_n);
    this->PacketIndex += _n;
    return str;
}



