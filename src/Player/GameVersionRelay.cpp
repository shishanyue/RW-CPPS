//
// Created by 21553 on 2023/7/30.
//

#include "Player/GameVersionRelay.h"

GameVersionRelay::GameVersionRelay(ConnectionInfo *connectionInfo,Cache *cache,map<Socket, GameVersionRelay> *connectionMap,RelayMap *_relayMap,map<uint32_t, vector<GameVersionRelay *>> *netGroupMap) : Connection(connectionInfo,cache) {
    this->ConnectionMap = connectionMap;
    this->relayMap = _relayMap;
    this->NetGroupMap = netGroupMap;
}


void GameVersionRelay::setSendPacketFunction(bool (*_sendPacketFunction)(GameVersionRelay *, AbstractDataStream *)) {
    this->sendPacketFunction = _sendPacketFunction;
}

bool GameVersionRelay::sendRelayServerInfo() {
    GameOutputStream o(PacketType::RELAY_VERSION_INFO);

    o.writeByte(0);
    // RELAY Version  151-114 176-115
    o.writeInt(151);
    // ?
    o.writeInt(1);
    // ?
    o.writeBool(false);
    sendPacketFunction(this,&o);
    return true;
}

bool GameVersionRelay::sendVerifyClientValidity() {
    netConnectAuthenticate = make_shared<NetConnectProofOfWork>();

    try {
        GameOutputStream o(PacketType::RELAY_POW);

        o.writeInt(netConnectAuthenticate->resultInt);
        uint32_t authenticateType = (netConnectAuthenticate->authenticateType);
        o.writeInt(authenticateType);

        if(authenticateType==0||(authenticateType>=2&&authenticateType<=4)||authenticateType==6){
            o.writeBool(true);
            o.writeInt(netConnectAuthenticate->initInt_1);
        } else{
            o.writeBool(false);
        }
        if(authenticateType==1||(authenticateType>=2&&authenticateType<=4)){
            o.writeBool(true);
            o.writeInt(netConnectAuthenticate->initInt_2);
        }else{
            o.writeBool(false);
        }
        if(authenticateType>=5&&authenticateType<=6){
            o.writeString(&netConnectAuthenticate->outcome);
            o.writeString(&netConnectAuthenticate->fixedInitial);
            o.writeInt(netConnectAuthenticate->maximumNumberOfCalculations);
        }
        o.writeBool(false);

        this->sendPacketFunction(this,&o);

    } catch(exception &e){
        return false;
    }
    return true;
}

inline bool GameVersionRelay::relayServerTypeInternal(const char *run) {
    string msg = ServerInfo().ServerInfoMsg;
    sendRelayHallMessage(&msg);
    connectionInfo.relaySelect = run;
    connectionInfo.inputPassword = true;
    return true;
}

inline bool GameVersionRelay::sendRelayHallMessage(string *str) {
    GameOutputStream o( PacketType::RELAY_117);
    o.writeByte(1);
    o.writeInt(5);
    o.writeString(str);
    sendPacketFunction(this,&o);
    return true;
}

inline bool GameVersionRelay::sendRelayHallMessage(const char *str) {
    GameOutputStream o( PacketType::RELAY_117);
    o.writeByte(1);
    o.writeInt(5);
    o.writeString(str);
    sendPacketFunction(this,&o);
    return true;
}

bool GameVersionRelay::relayDirectInspection(Relay *pRelay) {

    if(cachePacket.ContentLength!=0){

        GameInputStream gameInputStream(&cachePacket);
        gameInputStream.readString();

        uint32_t packetVersion = gameInputStream.readInt();

        connectionInfo.clientVersion = gameInputStream.readInt();

        connectionInfo.getBetaVersion();

        if(packetVersion>=1) gameInputStream.skin(4);

        string queryString;

        if(packetVersion>=2) queryString = gameInputStream.readIfIsString();

        if(packetVersion>=3) playerInfo.playerName = gameInputStream.readString();

        if(pRelay == nullptr){
            relayServerTypeInternal();
        }
    }
    return true;
}

bool GameVersionRelay::sendRelayServerTypeReply() {
    connectionInfo.inputPassword = false;

    string id = relayServerTypeReplyInternal(&inputStream);

    if(connectionInfo.relaySelect.empty()){
        idCustom(&id);
    }else{
        //TODO
    }
    return true;
}

inline string GameVersionRelay::relayServerTypeReplyInternal(GameInputStream *_inputStream) {
    //跳过无用的一个byte和一个int32
    _inputStream->skin(5);

    return _inputStream->readString();
}

inline void GameVersionRelay::idCustom(string *_id) {
    if(_id->empty()){
        sendRelayHallMessage("什么都没输入就点击确认");
    }

    if(StringUtils::startsWith(_id,"C")){
        Relay *_relay = relayMap->getRelay(*_id);
        if(_relay != nullptr){
            relay = _relay;
            addRelayConnect();
        }else{
            sendRelayHallMessage("此房间不存在");
        }
        return;
    }

    bool uplist = false;
    bool mods = false;
    bool newRoom = false;

    if(StringUtils::startsWith(_id,"new") || StringUtils::startsWith(_id,"news")){
        newRoom = true;
    }
    else if(StringUtils::startsWith(_id,"mod") || StringUtils::startsWith(_id,"mods")){
        newRoom = true;
        mods = true;
    }
    if(StringUtils::startsWith(_id,"up")){
        uplist =true;
    }

    CustomRelayData custom;

    if(newRoom){
        newRelayId(mods,uplist,&custom);
    }
}

inline bool GameVersionRelay::newRelayId(string *id, bool mod,bool upList, CustomRelayData *customRelayData) {
    uint32_t maxPlayer = customRelayData->MaxPlayerSize == -1? 10 : customRelayData->MaxPlayerSize;
    if(id == nullptr){
        relay = relayMap->getNewRelay("",playerInfo.playerName,
                                                  mod,upList,connectionInfo.betaGameVersion,
                                                connectionInfo.clientVersion,maxPlayer,
                                                  NetGroupMap);
    }

    //GameOutputStream o(PacketType::CHAT);

    //o.writeInt(101);
    //sendPacketFunction(this,&o);
    sendRelayServerId();

    relay->setAddSize(this);

    return true;
}

inline bool GameVersionRelay::newRelayId(bool mod,bool upList, CustomRelayData *customRelayData) {
    return newRelayId(nullptr,mod,upList,customRelayData);
}

inline bool GameVersionRelay::sendRelayServerId() {

    relay->admin = this;

    connectionInfo.permission = permissionStatus::HostPermission;

    GameOutputStream o(PacketType::RELAY_BECOME_SERVER);

    if(connectionInfo.clientVersion >= newRelayProtocolVersion){
        o.writeByte(2);
        o.writeBool(true);
        o.writeBool(true);
        o.writeBool(true);
        o.writeString("CPPS Team");
        o.writeBool(this->relay->isMod); //MOD
        o.writeBool(false);
        o.writeBool(true);

        o.writeString("{{RW-CPPS Relay}}.Room ID : C" + relay->id);
        //o.writeString("<>");
        o.writeBool(false);
        //registerPlayerId uuid
        o.writeIsString(UUIDUtils::GenerateUUID());
    }else{
        //TODO
    }
    sendPacketFunction(this,&o);
    return true;
}

bool GameVersionRelay::getPingData(GameInputStream *_inputStream) {
    GameOutputStream out(PacketType::HEART_BEAT_RESPONSE);

    out.writeBytes(inputStream.packet.bytesArray.data(),8);
    out.writeByte(1);
    out.writeByte(60);

    this->sendPacketFunction(this,&out);
    return true;
}


bool GameVersionRelay::disconnect() {

    if(relay!=nullptr){

        if(relay->admin!=nullptr&&this == relay->admin){

            if(relay->isStartGame){
                //TODO  adminMoveNew();
                uint32_t internalID = relay->internalID;
                string id = relay->id;
                auto netGroupMap = NetGroupMap;
                auto netGroup = relay->NetGroup;
                relayMap->removeRelay(id);
                for(auto it:*netGroup){
                    closesocket(it->connectionInfo.sock);
                    ConnectionMap->erase(it->connectionInfo.sock);
                }
                if(netGroupMap->find(internalID) != netGroupMap->end()){
                    netGroupMap->erase(internalID);
                }
            }
            else
            {
                uint32_t internalID = relay->internalID;
                string id = relay->id;
                auto netGroupMap = NetGroupMap;
                auto netGroup = relay->NetGroup;
                relayMap->removeRelay(id);
                for(auto it:*netGroup){
                    closesocket(it->connectionInfo.sock);
                    ConnectionMap->erase(it->connectionInfo.sock);
                }
                if(netGroupMap->find(internalID) != netGroupMap->end()){
                    netGroupMap->erase(internalID);
                }
                //relayMap->removeRelay(id);
            }

        } else{
            //Player断开
            if(relay->admin!=nullptr){
                sendPackageToHOST(&cache->packetCache["getExitPacket"]);
                if(relay->isStartGame){
                    relay->setRemoveSize(this);
                }
            }
            closesocket(connectionInfo.sock);

            if(ConnectionMap->find(connectionInfo.sock) != ConnectionMap->end()){
                //将
                ConnectionMap->erase(connectionInfo.sock);
            }
        }
    }else{
        closesocket(this->connectionInfo.sock);

        if(ConnectionMap->find(connectionInfo.sock) != ConnectionMap->end()){

            //需要在erase前关闭socket,因为ConnectionMap存储的不是指针
            // erase操作会删除这个连接
            ConnectionMap->erase(connectionInfo.sock);
        }
    }

    return true;
}

inline bool GameVersionRelay::addRelayConnect() {

    connectionInfo.permission = permissionStatus::PlayerPermission;

    connectionInfo.inputPassword = false;

    site = relay->setAddPosition();

    GameOutputStream o(PacketType::FORWARD_CLIENT_ADD);

    if(connectionInfo.clientVersion >= newRelayProtocolVersion){
        o.writeByte(1);

        o.writeInt(site);
        // ?
        o.writeString(UUIDUtils::GenerateUUID());
        // User UUID
        o.writeBool(false);

        o.writeIsString(connectionInfo.ip);

        sendPacketFunction(relay->admin,&o);
    }else{

    }

    sendPackageToHOST(&cachePacket);
    relay->setAddSize(this);
    chatMessagePacketInternal("欢迎QwQ","SERVER",5);
    return true;
}

bool GameVersionRelay::addRelaySend(GameInputStream *_inputStream) {
    uint32_t target = _inputStream->readInt();
    uint32_t type = _inputStream->readInt();

    if (type == (uint32_t)PacketType::DISCONNECT) {
        return false;
    }

    Packet packet(_inputStream->readStreamBytes(),PacketTypeUtils::ConvertToPacketType(type));

    GameVersionRelay *_con = relay->getConnect(target-1);

    if(type == (uint32_t)PacketType::KICK){
        GameOutputStream o(&packet);

        sendPacketFunction(_con,&o);
        //TODO relayPlayerDisconnect()
        return true;
    }

    if(type == (uint32_t)PacketType::START_GAME){
        relay->isStartGame = true;
    }


    GameOutputStream o(&packet);

    sendPacketFunction(_con,&o);
    return true;
}

bool GameVersionRelay::sendPackageToHOST(Packet *packet) const {
    GameOutputStream o(PacketType::PACKET_FORWARD_CLIENT_FROM);

    o.writeInt(site);
    o.writeInt(packet->bytesArray.size() + 8);
    o.writeInt(packet->bytesArray.size());
    o.writeInt((uint32_t)packet->packetType);
    o.writeBytes(packet->bytesArray.data(),packet->bytesArray.size());
    sendPacketFunction(relay->admin,&o);
    return true;
}

inline bool GameVersionRelay::chatMessagePacketInternal(const string& msg, const string& sendBy, uint32_t team) {
    GameOutputStream o(PacketType::CHAT);
    // The message contained in the package
    o.writeString(msg);
    // Protocol version (in older versions here 0 1 2 3 respectively)
    o.writeByte(3);
    // Whether the sender name is empty and write data
    o.writeIsString(sendBy);
    // Team
    o.writeInt(team);
    // Team
    o.writeInt(team);
    this->sendPacketFunction(this,&o);
    return true;
}

bool GameVersionRelay::receiveChat(Packet *packet) const {
    //TODO command
    sendPackageToHOST(packet);
    return true;
}





