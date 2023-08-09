//
// Created by 21553 on 2023/7/31.
//

#include "Cache/Cache.h"

Cache::Cache() {
    this->packetCache.insert(
            make_pair("getExitPacket",
                      Packet("exited",PacketType::DISCONNECT
                      )));

}
