//
// Created by 21553 on 2023/6/23.
//

#ifndef RW_CPS_SERVERINFO_H
#define RW_CPS_SERVERINFO_H

#include <iostream>
#include <cstring>

using namespace std;

enum class ServerType{
    TCP,
    UDP,
    Unknown
};

class ServerInfo{
public:
    ServerInfo(){
        this->port = 0;
        this->serverType = ServerType::Unknown;
    };
    ServerInfo(u_short port,ServerType serverType){
        this->port = port;
        this->serverType = serverType;
    };
    string ServerInfoMsg = "[Relay-CPPS] 这台服务器是CN非官方的Relay room\n由十山月根据RW-HPS使用C++重写\nRW-HPS github地址:https://github.com/RW-HPS/RW-HPS";
    u_short port;
    ServerType serverType;
};


#endif //RW_CPS_SERVERINFO_H
