//
// Created by 21553 on 2023/7/11.
//

#ifndef RW_CPPS_SERVERBRIDGE_H
#define RW_CPPS_SERVERBRIDGE_H
#include <map>
#include <string>
#include <memory>

#include "ThreadPool/ThreadPool.h"
#include "Player/Connection.h"
#include "TypeRelay/TypeRelay.h"
#include "DataStream/AbstractDataStream.h"
#include "NetServer/global/RelayMap.h"
#include "Cache/Cache.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>
#include <mswsock.h>

typedef SOCKET Socket;
#pragma comment(lib,"Mswsock.lib")

//定义枚举类型
enum IO_EVENT{
    IO_ACCEPT,
    IO_RECV_PACKET_HEAD,
    IO_RECV_PACKET_DATA,
    IO_SEND
};
//定义具体重叠结构体

struct PacketYOV :public OVERLAPPED
{
    ~PacketYOV(){
        if(m_event == IO_RECV_PACKET_HEAD || m_event == IO_RECV_PACKET_DATA){
            delete m_btBuf;
        }
    };
    PacketYOV(Socket sock, IO_EVENT event,size_t dataSize) : _OVERLAPPED()
    {
        /*
        //char *data = new char[dataSize];
        data = make_shared<char>(dataSize);
        //data.append(dataSize,0);
        memset(&*data, 0, sizeof(*data));
        m_sockClient = sock;
        m_btBuf = &*data;
        m_buf.buf = m_btBuf;
        m_buf.len = dataSize;
        m_dwBytesRecved = 0;
        m_dwFlag = 0;
        m_event = event;*/


        char *data = new char[dataSize];
        //data = make_shared<char>(dataSize);
        //data.append(dataSize,0);
        memset(data, 0, sizeof(*data));
        m_sockClient = sock;
        m_btBuf = data;
        m_buf.buf = m_btBuf;
        m_buf.len = dataSize;
        m_dwBytesRecved = 0;
        m_dwFlag = 0;
        m_event = event;
    }
    PacketYOV(Socket sock, IO_EVENT event,char *buffer,size_t bufferSize) : _OVERLAPPED()
    {
        m_sockClient = sock;
        m_btBuf = buffer;
        m_buf.buf = m_btBuf;
        m_buf.len = bufferSize;
        m_dwBytesRecved = 0;
        m_dwFlag = 0;
        m_event = event;

    }
    IO_EVENT m_event;
    Socket m_sockClient;
    WSABUF m_buf{};
    CHAR *m_btBuf;
    DWORD m_dwBytesRecved;
    DWORD m_dwFlag;
    //shared_ptr<char> data{};
};


struct AcceptYOV :public OVERLAPPED
{
    AcceptYOV(Socket sock, IO_EVENT event) : _OVERLAPPED()
    {
        //char *data = new char[dataSize];
        //data.append(dataSize,0);
        //memset(data, 0, sizeof(*data));
        m_sockClient = sock;
        m_btBuf = reinterpret_cast<CHAR *>(&m_Bufs);
        m_buf.buf = m_btBuf;
        m_buf.len = 0;
        m_dwBytesRecved = 0;
        m_dwFlag = 0;
        m_event = event;
        memset(m_Bufs.m_pRemote, 0, sizeof(m_Bufs.m_pRemote));
        memset(m_Bufs.m_pLocal, 0, sizeof(m_Bufs.m_pLocal));
    }
    IO_EVENT m_event;
    Socket m_sockClient;
    WSABUF m_buf{};
    CHAR *m_btBuf;
    DWORD m_dwBytesRecved;
    DWORD m_dwFlag;
    struct {
        BYTE m_pLocal[sizeof(SOCKADDR_IN) + 16];
        BYTE m_pRemote[sizeof(SOCKADDR_IN) + 16];
    } m_Bufs{};
};



#else
// win32
#endif
#elif __linux__
//TODO
#endif



using namespace std;

class ServerBridge{
public:
    virtual void StartServer(ThreadPool& threadPool,size_t threadSize,map<Socket ,GameVersionRelay> *connectionMap,RelayMap *relayMap,map<uint32_t , vector<GameVersionRelay*>> *netGroupMap,Cache *cache)=0;
    map<Socket , GameVersionRelay> *connectionMap{};
    RelayMap *relayMap{};
    map<uint32_t , vector<GameVersionRelay*>> *netGroupMap{};
    Cache *cache{};
};

#endif //RW_CPPS_SERVERBRIDGE_H

