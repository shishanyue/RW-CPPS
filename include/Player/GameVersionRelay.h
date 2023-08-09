//
// Created by 21553 on 2023/7/30.
//

#ifndef RW_CPPS_GAMEVERSIONRELAY_H
#define RW_CPPS_GAMEVERSIONRELAY_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
#include "winsock2.h"
// win64
typedef SOCKET Socket;
#else
// win32
#endif
#elif __linux__
#include <sys/socket.h>
#endif


#include "ServerInfo/ServerInfo.h"
#include "DataStream/GameInputStream.h"
#include "DataStream/GameOutputStream.h"
#include "Connection.h"
#include "NetServer/global/RelayMap.h"

static const uint32_t newRelayProtocolVersion = 172;


class GameVersionRelay:public Connection {
public:
    GameVersionRelay(ConnectionInfo *connectionInfo,Cache *cache,map<Socket, GameVersionRelay> *connectionMap,RelayMap *_relayMap,map<uint32_t , vector<GameVersionRelay *>> *netGroupMap);
    GameInputStream inputStream;
    Relay *relay = nullptr;

    //void setNetServer(class NetServer *_netServer);

    bool sendRelayServerInfo();
    bool sendVerifyClientValidity();
    bool relayServerTypeInternal(const char *run = "");

    bool sendRelayHallMessage(string *str);
    bool sendRelayHallMessage(const char *str);

    bool chatMessagePacketInternal(const string& msg,const string& sendBy,uint32_t team);

    bool relayDirectInspection(Relay *pRelay);

    void setSendPacketFunction(bool (*_sendPacketFunction)(GameVersionRelay *con, AbstractDataStream *dataStream));

    bool sendRelayServerTypeReply();

    void idCustom(string *_id);

    static string relayServerTypeReplyInternal(GameInputStream *_inputStream);
    bool newRelayId(bool mod,bool upList,CustomRelayData *customRelayData);

    bool addRelayConnect();

    bool addRelaySend(GameInputStream *_inputStream);
    bool getPingData(GameInputStream *_inputStream);

    bool sendPackageToHOST(Packet *packet) const;
    bool receiveChat(Packet *packet) const;
    bool disconnect();


    map<Socket, GameVersionRelay> *ConnectionMap;
    RelayMap *relayMap;
    map<uint32_t, vector<GameVersionRelay *>> *NetGroupMap;

private:
    bool sendRelayServerId();
    bool (*sendPacketFunction)(GameVersionRelay *con, AbstractDataStream *dataStream){};
    bool newRelayId(string *id,bool mod,bool upList,CustomRelayData *customRelayData);




};


#endif //RW_CPPS_GAMEVERSIONRELAY_H
