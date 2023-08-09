//
// Created by 21553 on 2023/7/30.
//

#ifndef RW_CPPS_RELAYMAP_H
#define RW_CPPS_RELAYMAP_H
#include "NetServer/global/Relay.h"

class RelayMap{
public:
    Relay *getNewRelay(const string& id,const string& playerName,bool isMod,bool upList,bool betaGameVersion,uint32_t version,uint32_t maxPlayer,map<uint32_t , vector<GameVersionRelay*>> *netGroup);
    Relay *getRelay(const string& id);
    bool removeRelay(const string& id);
    map<string ,Relay> RelayData;
private:
    static uint32_t coverRelayID(string& id);
};
#endif //RW_CPPS_RELAYMAP_H
