//
// Created by 21553 on 2023/7/20.
//

#ifndef RW_CPPS_RELAY_H
#define RW_CPPS_RELAY_H

#include <cinttypes>
#include <map>
#include <vector>
#include <atomic>
#include "utils/utils.h"
#include <memory>

class GameVersionRelay;

using namespace std;

class Relay{
public:
    Relay(uint32_t internalID,const string& id,const string& playerName,bool isMod,bool upList,bool betaGameVersion,uint32_t version,uint32_t maxPlayer,vector<class GameVersionRelay *> *netGroup);

    ~Relay();
    uint32_t internalID;
    bool isMod;
    bool isStartGame = false;
    string id;
    bool upList = false;
    bool setAddSize(GameVersionRelay *con) const;

    //uint32_t site = 1;
    shared_ptr<atomic<uint32_t >> site = make_shared<atomic<uint32_t>>(1);;
    //atomic<uint32_t> *site;
    [[nodiscard]] uint32_t setAddPosition() const;
    GameVersionRelay *admin{};
    [[nodiscard]] GameVersionRelay * getConnect(uint32_t _site) const;
    vector<GameVersionRelay *> *NetGroup;
    bool setRemoveSize(GameVersionRelay *con) const;
};



#endif //RW_CPPS_RELAY_H
