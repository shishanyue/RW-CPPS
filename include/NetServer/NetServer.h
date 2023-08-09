//
// Created by 21553 on 2023/7/10.
//

#ifndef RW_CPPS_NETSERVER_H
#define RW_CPPS_NETSERVER_H
#include <string>
#include <map>
#include <memory>
#include "ThreadPool/ThreadPool.h"
#include "NetServer/code/ServerBridge.h"
#include "NetServer/code/tcp/TcpServer.h"
//#include "NetServer/code/udp/UdpServer.h"
#include "Player/GameVersionRelay.h"
#include "Cache/Cache.h"

#include "ServerInfo/ServerInfo.h"
using namespace std;



class NetServer {
public:
    explicit NetServer(const ServerInfo& serverInfo,unsigned int threadSize = 0);
    ~NetServer();

    map<Socket, GameVersionRelay> ConnectionMap;
    RelayMap relayMap;
    map<uint32_t, vector<GameVersionRelay *>> NetGroupMap;
    Cache cache;
    bool StartServer();
    ServerInfo serverInfo{};
private:
    shared_ptr<ServerBridge> server;

    unsigned int threadSize;

    shared_ptr<ThreadPool> threadPool;

};


#endif //RW_CPPS_NETSERVER_H
