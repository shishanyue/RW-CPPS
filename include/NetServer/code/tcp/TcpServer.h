//
// Created by 21553 on 2023/7/10.
//

#ifndef RW_CPPS_TCPSERVER_H
#define RW_CPPS_TCPSERVER_H
#include <iostream>
#include <string>

#include "DataStream/AbstractDataStream.h"
#include "NetServer/code/ServerBridge.h"
#include "Player/GameVersionRelay.h"

using namespace std;

class TcpServer: public ServerBridge{
public:
    explicit TcpServer(unsigned int listenPort);

    ~TcpServer();

    void StartServer(ThreadPool &fThreadPool,size_t _threadSize,
                     map<Socket , GameVersionRelay> *_connectionMap,
                     RelayMap *relayMap,
                     map<uint32_t , vector<GameVersionRelay*>> *netGroupMap,Cache *cache) override;

private:

    unsigned int ListenPort;

    Socket ListenSocket{};

    size_t threadSize{};
    ThreadPool *threadPool{};

    static bool sendPacket(GameVersionRelay *con, AbstractDataStream *dataStream);

    [[noreturn]] static void ThreadWorker(TcpServer *tcpServer);


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
        //完成端口
    HANDLE hIOCompletionPort;

    LPFN_ACCEPTEX fAcceptEx{};

    void PostAccept(Socket socketListen, HANDLE hIocp);
    static void PostRecv(Socket sock, IO_EVENT event, size_t _dataSize);
    static bool getConnectionAddr(AcceptYOV *acceptYov, SOCKADDR_IN *conAddr);
#else
        // win32
#endif
#elif __linux__
        //TODO
#endif

};


#endif //RW_CPPS_TCPSERVER_H
