//
// Created by 21553 on 2023/6/22.
//

#ifndef RWSERVER_CONNECTION_H
#define RWSERVER_CONNECTION_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>
#include <mswsock.h>
#pragma comment(lib,"Mswsock.lib")
// win64
typedef SOCKET Socket;
#else
// win32
#endif
#elif __linux__
#include <sys/socket.h>
#endif

#include "NetServer/protocol/realize/NetConnectProofOfWork.h"
#include "Packet/Packet.h"
#include <string>
#include <utility>
#include "NetServer/global/CustomRelayData.h"
#include "Cache/Cache.h"
#include <memory>

using namespace std;

enum class permissionStatus{
    InitialConnection,
    GetPlayerInfo,
    WaitCertified,
    CertifiedEnd,
    PlayerPermission,
    HostPermission,
    Debug,
    Null,
};

enum class ipCountry{
    zh,
    unknown,
};

typedef struct ConnectionInfo{
    ConnectionInfo(){
        this->sock = -1;
        this->permission = permissionStatus::Null;
        this->ip = "";
    }
    ConnectionInfo(Socket sock,permissionStatus permission,sockaddr_in *con_addr_in,string ip){
        this->sock = sock;
        this->permission = permission;
        this->con_addr_in = *con_addr_in;
        this->ip = std::move(ip);
    }
    permissionStatus permission = permissionStatus::Null;
    sockaddr_in con_addr_in{};
    string ip;
    Socket sock;
    uint32_t clientVersion = 151;
    bool betaGameVersion = false;
    bool inputPassword = false;
    string relaySelect;
    void getBetaVersion(){
        betaGameVersion = clientVersion>=152&&clientVersion<=175;
    };
}ConnectionInfo;

typedef struct PlayerInfo{
    PlayerInfo(){
        this->playerName = "";
        this->ipCountry = ipCountry::unknown;
    }
    PlayerInfo(string& playerName,ipCountry ipCountry){
        this->playerName = playerName;
        this->ipCountry = ipCountry;
    }
    string playerName;
    ipCountry ipCountry;
}PlayerInfo;


class Connection {
public:
    Connection(ConnectionInfo *connectionInfo,Cache *cache);

    ~Connection();

    ConnectionInfo connectionInfo;

    shared_ptr<NetConnectProofOfWork> netConnectAuthenticate;
    PlayerInfo playerInfo;

    Packet cachePacket;
    Cache *cache;
    uint32_t site = 0;
    void setCachePacket(Packet *packet);

};


#endif //RWSERVER_CONNECTION_H
