//
// Created by 21553 on 2023/7/10.
//
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
#include <sysinfoapi.h>
// win64
#else
// win32
#endif
#elif __linux__
//TODO
#endif


#include "NetServer/NetServer.h"



NetServer::NetServer(const ServerInfo& serverInfo,unsigned int threadSize) {
    this->serverInfo = serverInfo;

    if(threadSize == 0) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64

        SYSTEM_INFO si;
        GetSystemInfo(&si);
        this->threadSize = si.dwNumberOfProcessors * 2;
        //threadSize=2;
#else
        // win32
#endif
#elif __linux__
        //TODO
#endif
    }else{
        this->threadSize = threadSize;
    }
    this->threadPool = make_shared<ThreadPool>(threadSize);

    switch (this->serverInfo.serverType) {
        case ServerType::TCP:
            server = make_shared<TcpServer>(this->serverInfo.port);
            break;
            /*
        case ServerType::UDP:
            server = make_shared<UdpServer>(ListenPort);
            break;*/
        case ServerType::UDP:
            break;
        case ServerType::Unknown:
            break;
    }
}

bool NetServer::StartServer() {
    this->server->StartServer(*threadPool,threadSize,&ConnectionMap,&relayMap,&NetGroupMap,&cache);
    return true;
}

NetServer::~NetServer() {
    threadPool.reset();
}




