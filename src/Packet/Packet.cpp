//
// Created by 21553 on 2023/6/22.
//

#include <utility>

#include "Packet/Packet.h"
Packet::Packet() {
    this->packetType = PacketType::NOT_RESOLVED;
}

Packet::Packet(PacketType packetType) {
    this->packetType = packetType;
}

Packet::Packet(BytesArray *_bytesArray) {
    this->packetType = PacketType::NOT_RESOLVED;
    this->bytesArray = *_bytesArray;
    this->ContentLength = this->bytesArray.size();
}

Packet::Packet(BytesArray _bytesArray) {
    this->packetType = PacketType::NOT_RESOLVED;
    this->bytesArray = std::move(_bytesArray);
    this->ContentLength = this->bytesArray.size();
}

Packet::Packet(BytesArray _bytesArray, PacketType packetType) {
    this->packetType = packetType;
    this->bytesArray = std::move(_bytesArray);
    this->ContentLength = this->bytesArray.size();
}

bool Packet::Write(const char *src,size_t _n) {
    try {
        //将数据复制到末尾
        this->bytesArray.append(src, _n);
        this->ContentLength += _n;
        return true;
    } catch(exception& e) {
        return false;
    }
}

bool Packet::Prepare() {
    try {
        //  |0   1   2   3|4   5   6   7|8 9 10 11
        //  ContentLength |packetType   |Data
        // 按照顺序将contentLength packetType从0放入
        uint32_t _packetType = ntohl(static_cast<uint32_t>(this->packetType));
        uint32_t _contentLength = ntohl(this->ContentLength);
        this->bytesArray.insert(0, reinterpret_cast<const char *>(&_packetType), sizeof(uint32_t));
        this->bytesArray.insert(0, reinterpret_cast<const char *>(&_contentLength), sizeof(uint32_t));
        return true;
    } catch(exception& e) {
        return false;
    }
}






/*Cache::Cache() {

    this->packetCache.insert(make_pair("getExitPacket",
                                       Packet("exited",
                                              PacketType::DISCONNECT)));
}*/
