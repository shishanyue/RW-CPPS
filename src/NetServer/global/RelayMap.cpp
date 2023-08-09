//
// Created by 21553 on 2023/7/30.
//

#include "NetServer/global/RelayMap.h"

Relay *RelayMap::getNewRelay(const string& id,const string& playerName, bool isMod,bool upList, bool betaGameVersion, uint32_t version,
                             uint32_t maxPlayer,map<uint32_t , vector<GameVersionRelay *>> *netGroupMap) {
    RandomUtils randomUtils;
    string idRelay = id;
    if(id.empty()){

        uint32_t intId =randomUtils.randomInt32(1000,100000);

        while(RelayData.find(to_string(intId)) != RelayData.end()){
            intId =randomUtils.randomInt32(1000,100000);
        }
        idRelay = to_string(intId);

        auto it = netGroupMap->insert(make_pair(coverRelayID(idRelay),
                                                vector<GameVersionRelay*>()));

        vector<GameVersionRelay *> *_netGroup = &it.first->second;

        auto relayIt = RelayData.insert(make_pair(idRelay,Relay(coverRelayID(idRelay),
                                                                     idRelay,playerName,isMod,upList,
                                                                     betaGameVersion,version,
                                                                     maxPlayer,_netGroup)));

        //relayIt.first->second.NetGroupId = it.first->first;

        return &relayIt.first->second;
        //netGroupMap->insert();
    } else{

    }
    return nullptr;
}

uint32_t RelayMap::coverRelayID(string &id) {

    if(StringUtils::isNumeric(id) && id.length() < 10){
        return atoi(id.data());
    } else{
        string rId;
        picosha2::hash256_hex_string(id,rId);

        auto it=rId.begin();

        for(;it!=rId.end();it++){
            *it = (int)(*it)>0?(char)((int)(*it)*-1):*it;
            //rId.replace(it,(int)(*it)>0?*it:*it);
        }
    }
    return 0;
}

Relay *RelayMap::getRelay(const string &id) {
    return RelayData.find(id) == RelayData.end()?nullptr:&RelayData.at(id);
}

bool RelayMap::removeRelay(const string &id) {

    if(RelayData.find(id)!=this->RelayData.end()){
        //delete RelayData.at(id).site;
        RelayData.erase(id);
    }

    return true;
}