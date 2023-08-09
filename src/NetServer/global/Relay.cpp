//
// Created by 21553 on 2023/7/20.
//

#include "NetServer/global/Relay.h"

Relay::Relay(uint32_t internalID,const string &id,const string &playerName, bool isMod,bool upList, bool betaGameVersion, uint32_t version,
             uint32_t maxPlayer,vector<GameVersionRelay *> *netGroup) {
    this->internalID = internalID;
    this->id = id;
    this->isMod = isMod;
    this->NetGroup = netGroup;
    this->upList = upList;
}

Relay::~Relay() = default;

bool Relay::setAddSize(GameVersionRelay *con) const {
    NetGroup->push_back(con);
    return true;
}

uint32_t Relay::setAddPosition() const {
    return site==nullptr?-1:++(*site);
}

GameVersionRelay *Relay::getConnect(uint32_t _site) const {
    return _site > NetGroup->size()? nullptr: NetGroup->at(_site);
}

bool Relay::setRemoveSize(GameVersionRelay *con) const {
    auto it = NetGroup->begin();
    for(;it!=NetGroup->end();it++){
        if(it+1==NetGroup->end()) return false;
        if(it.base() == &con){
            NetGroup->erase(it);
            return true;
        }
    }
    return true;
}








