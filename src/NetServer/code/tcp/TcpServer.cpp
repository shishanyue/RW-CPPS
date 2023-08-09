//
// Created by 21553 on 2023/7/10.
//

#include "NetServer/code/tcp/TcpServer.h"

TcpServer::TcpServer(unsigned int listenPort) {
    //端口
    this->ListenPort = listenPort;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
    //Windows的完成端口
    this->hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
#else
    // win32
#endif
#elif __linux__
    //TODO
#endif

}


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
inline void TcpServer::PostRecv(Socket sock,IO_EVENT event,size_t _dataSize) {
//接收数据的请求
    auto* pOv = new PacketYOV(sock, event,_dataSize);
    //等待接收全部数据
    pOv->m_dwFlag = MSG_WAITALL;
    //int nRet =
    WSARecv(sock,&pOv->m_buf, 1,
            &pOv->m_dwBytesRecved,&pOv->m_dwFlag,
            pOv,nullptr);
}

inline void TcpServer::PostAccept(Socket socketListen, HANDLE hIocp) {
    //提前准备连接的Socket句柄
    Socket sockClient = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);


    //绑定到完成端口
    CreateIoCompletionPort((HANDLE) sockClient, hIocp, 0, 0);
    //
    DWORD dwRecved = 0;

    auto *pOv = new AcceptYOV(sockClient,IO_ACCEPT);

    this->fAcceptEx(socketListen,sockClient,&pOv->m_Bufs,0,sizeof(pOv->m_Bufs.m_pLocal),sizeof(pOv->m_Bufs.m_pRemote),&dwRecved,pOv);

    //delete pOv;
}

inline bool TcpServer::sendPacket(GameVersionRelay *con, AbstractDataStream *dataStream) {
    dataStream->packet.Prepare();
    BytesArray *bytesArray = &dataStream->packet.bytesArray;
    auto *sendYOV = new PacketYOV(con->connectionInfo.sock,IO_SEND,bytesArray->data(),bytesArray->size());

    DWORD numberOfBytesSent = 0;

    int socket_error = WSASend(sendYOV->m_sockClient,&sendYOV->m_buf,1,&numberOfBytesSent,0,sendYOV,nullptr);
    if(socket_error!=0){
        con->disconnect();
    }
    return true;
}

inline bool TcpServer::getConnectionAddr(AcceptYOV *acceptYov,SOCKADDR_IN *conAddr) {
    *conAddr = *(sockaddr_in *)acceptYov->m_Bufs.m_pRemote;
    return true;
}
#else
// win32
#endif
#elif __linux__
//TODO
#endif

TcpServer::~TcpServer() {
    while (WSACleanup()!=0);
}

void TcpServer::StartServer(ThreadPool& fThreadPool,size_t _threadSize,
                            map<Socket , GameVersionRelay> *_connectionMap,RelayMap *relayMap,map<uint32_t , vector<GameVersionRelay*>> *netGroupMap,Cache *cache) {
    this->connectionMap = _connectionMap;
    this->threadPool = &fThreadPool;
    this->threadSize = _threadSize;
    this->relayMap = relayMap;
    this->netGroupMap = netGroupMap;
    this->cache = cache;



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
    // 初始化Socket库
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    //初始化Socket
    // 这里需要特别注意，如果要使用重叠I/O的话，这里必须要使用WSASocket来初始化Socket
    // 注意里面有个WSA_FLAG_OVERLAPPED参数
    this->ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);


    //获取AcceptEx函数的指针
    GUID GuidAcceptEx = WSAID_ACCEPTEX;        // GUID，这个是识别AcceptEx函数必须的
    DWORD dwBytes = 0;

    WSAIoctl(
            ListenSocket,
            SIO_GET_EXTENSION_FUNCTION_POINTER,
            &GuidAcceptEx,
            sizeof(GuidAcceptEx),
            &fAcceptEx,
            sizeof(fAcceptEx),
            &dwBytes,
            nullptr,
            nullptr);


    struct sockaddr_in ServerAddress{};
    // 填充地址结构信息
    ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    // 这里可以选择绑定任何一个可用的地址，或者是自己指定的一个IP地址
    ServerAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    ServerAddress.sin_port = htons(ListenPort);

    // 绑定端口
    if (SOCKET_ERROR == bind(ListenSocket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress))){

    }
    // 开始监听
    listen(ListenSocket,SOMAXCONN);

    //绑定监听Socket和完成端口
    CreateIoCompletionPort((HANDLE)ListenSocket,hIOCompletionPort,(DWORD)this->ListenSocket,0);

    PostAccept(ListenSocket,hIOCompletionPort);


    for(int i=0;i<threadSize;i++){
        threadPool->enqueue([](TcpServer *tcpServer){
            ThreadWorker(tcpServer);
        },this
        );
    }
    ThreadWorker(this);
#else
    // win32
#endif
#elif __linux__
    //TODO
#endif
}

[[noreturn]] void TcpServer::ThreadWorker(TcpServer *tcpServer) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
    Socket listenSocket = tcpServer->ListenSocket;
    HANDLE *hIOCP = &tcpServer->hIOCompletionPort;
    map<Socket , GameVersionRelay> *connectionMap = tcpServer->connectionMap;

    DWORD dwBytesTranfered = 0;
    ULONG_PTR uKey;
    LPOVERLAPPED pOv = nullptr;
    bool status;

    //接下来要接收的数据大小
    uint32_t contentLength;
    //这个Packet的类型
    PacketType packetType;

    GameVersionRelay *con;
    while (true)
    {
        //等待完成端口

        status = GetQueuedCompletionStatus(
                *hIOCP,
                &dwBytesTranfered,
                &uKey,
                &pOv,
                INFINITE);
        auto *pov = (PacketYOV*)pOv;
        if(!status){
            if(connectionMap->find(pov->m_sockClient)!=connectionMap->end()){
                connectionMap->at(pov->m_sockClient).disconnect();
            }
            switch (pov->m_event) {
                case IO_ACCEPT:
                    delete (AcceptYOV *)pOv;
                    break;
                case IO_SEND:
                case IO_RECV_PACKET_HEAD:
                case IO_RECV_PACKET_DATA:
                    delete (PacketYOV *)pOv;
                    break;
            }
            //delete pOv;
            continue;
        }

        switch (pov->m_event)
        {
            //接收新的连接
            case IO_ACCEPT: {
                //连接完成后，再次投递一个连接的请求
                tcpServer->PostAccept(listenSocket, *hIOCP);

                //获取连接的IP
                SOCKADDR_IN sockaddrIn;
                TcpServer::getConnectionAddr((AcceptYOV*)pOv,&sockaddrIn);

                //建立新的连接
                ConnectionInfo connectionInfo(pov->m_sockClient
                                              ,permissionStatus::InitialConnection,&sockaddrIn
                                              ,inet_ntoa(sockaddrIn.sin_addr));
                //验证

                //成功建立新的连接
                GameVersionRelay connection = GameVersionRelay(&connectionInfo,tcpServer->cache,connectionMap,tcpServer->relayMap,tcpServer->netGroupMap);


                //获取发送Packet的函数指针
                connection.setSendPacketFunction(TcpServer::sendPacket);

                //添加到连接列表
                connectionMap->insert(pair<Socket, GameVersionRelay>(pov->m_sockClient, connection));

                //投递一个接收Packet的ContentLength与PacketType的请求
                TcpServer::PostRecv(pov->m_sockClient,IO_RECV_PACKET_HEAD,PACKET_HEAD_SIZE);
                delete (AcceptYOV *)pOv;
                break;
            }
                //Packet HEAD
            case IO_RECV_PACKET_HEAD: {

                //将读取到的int32转成PacketType
                packetType = PacketTypeUtils::ConvertToPacketType(htonl(*(uint32_t *) (pov->m_buf.buf +sizeof(uint32_t))));
                //大小端转换
                contentLength = htonl(*(uint32_t *) pov->m_buf.buf);


                if(connectionMap->find(pov->m_sockClient)==connectionMap->end()){
                    closesocket(pov->m_sockClient);
                    break;
                }
                con = &connectionMap->at(pov->m_sockClient);
                con->inputStream.packet.packetType = packetType;
                //投递一个接收Packet剩余数据的请求
                TcpServer::PostRecv(pov->m_sockClient,IO_RECV_PACKET_DATA,contentLength);
                delete pov;
                break;
            }

                //接收到Packet剩余数据
            case IO_RECV_PACKET_DATA:{
                //获取该客户端
                if(connectionMap->find(pov->m_sockClient)==connectionMap->end()){
                    closesocket(pov->m_sockClient);
                    break;
                }
                con = &(connectionMap->at(pov->m_sockClient));
                //con->PrepareNewPacket(pov->m_buf.len);
                //存放Packet剩余数据
                con->inputStream.prepareNewPacketBuff();
                con->inputStream.packet.Write(pov->m_btBuf,pov->m_buf.len);
                //处理请求

                /*
                tcpServer->threadPool->enqueue([](map<Socket , GameVersionRelay> *connectionMap, GameVersionRelay *_con){
                    return TypeRelay::TypeRelayCheck(connectionMap,_con);
                },tcpServer->connectionMap,con);*/


                TypeRelay::TypeRelayCheck(connectionMap,con);
                //重新投递一个接收Packet剩的ContentLength与PacketType的请求
                TcpServer::PostRecv(pov->m_sockClient,IO_RECV_PACKET_HEAD,PACKET_HEAD_SIZE);
                delete pov;
                break;
            }
            case IO_SEND:
                delete pov;
                break;
            default:
                if(connectionMap->find(pov->m_sockClient)!=connectionMap->end()){
                    connectionMap->at(pov->m_sockClient).disconnect();
                }else{
                    closesocket(pov->m_sockClient);
                }
                delete pOv;
                break;
        }
    }
#else
    // win32
#endif
#elif __linux__
    //TODO
#endif
}










