#include "Player/Connection.h"

#include <utility>

Connection::Connection(ConnectionInfo *connectionInfo,Cache *cache){
    this->connectionInfo = *connectionInfo;
    this->cache = cache;
}

Connection::~Connection() = default;

void Connection::setCachePacket(Packet *packet) {
    this->cachePacket = *packet;
};
















