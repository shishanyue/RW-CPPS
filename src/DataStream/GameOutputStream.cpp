//
// Created by 21553 on 2023/6/22.
//

#include "DataStream/GameOutputStream.h"

GameOutputStream::GameOutputStream(const Packet *packet,const PacketType packetType){

    this->packet.packetType = packetType;

    if(packet != nullptr){

        this->packet.bytesArray = packet->bytesArray;
        this->packet.ContentLength = this->packet.bytesArray.size();

    }
}

GameOutputStream::GameOutputStream(const Packet *packet) {
    if(packet != nullptr){
        this->packet = *packet;
    }
}

GameOutputStream::GameOutputStream(const PacketType packetType) {
    this->packet.packetType = packetType;
}


void GameOutputStream::writeInt(const uint32_t value) {
    uint32_t _value = ntohl(value);
    this->packet.Write(reinterpret_cast<const char *>(&_value), sizeof _value);
}

void GameOutputStream::writeShort(const uint16_t value) {
    uint16_t _value = ntohs(value);
    this->packet.Write(reinterpret_cast<const char *>(&_value), sizeof _value);
}

void GameOutputStream::writeString(const string *value) {
    uint16_t size = value->size();
    writeShort(size);
    this->packet.Write(reinterpret_cast<const char *>(value->data()), size);
}

void GameOutputStream::writeString(const string& value) {
    uint16_t size = value.size();
    writeShort(size);
    this->packet.Write(reinterpret_cast<const char *>(value.data()), size);
}

void GameOutputStream::writeByte(const uint8_t value) {
    this->packet.Write(reinterpret_cast<const char *>(&value), sizeof value);
}

void GameOutputStream::writeBool(const bool value) {
    this->packet.Write(reinterpret_cast<const char *>(&value), sizeof value);
}

void GameOutputStream::writeString(const char *value) {
    uint16_t size = strlen(value);
    writeShort(size);
    this->packet.Write(value, size);
}

void GameOutputStream::writeBytes(const void *src, size_t _n) {
    this->packet.Write(reinterpret_cast<const char *>(src), _n);
}

void GameOutputStream::writeIsString(const string *value) {
    if(value != nullptr){
        if(value->empty()){
            writeBool(false);
        }else{
            writeBool(true);
            writeString(value);
        };
    }else{
        writeBool(true);
        writeString(value);
    };
}

void GameOutputStream::writeIsString(const string& value) {
    if(value.empty()){
        writeBool(false);
    }else{
        writeBool(true);
        writeString(value);
    };
}

void GameOutputStream::writeIsString(const char *value) {
    if(strlen(value) == 0){
        writeBool(false);
    }else{
        writeBool(true);
        writeString(value);
    }
}









