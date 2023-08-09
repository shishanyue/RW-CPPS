#include "NetServer/NetServer.h"
#include <iostream>
#include "ThreadPool/ThreadPool.h"

using namespace std;
int main() {


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
#include <windows.h>

    SetConsoleOutputCP(65001);
// win64
#else
    // win32
#endif
#elif __linux__
    //TODO
#endif
    unsigned int port;
    unsigned int threadSize;
    cin >> port;
    cin >> threadSize;

    ServerInfo serverInfo(port,ServerType::TCP);
    cout << "RW-CPPS 启动！" <<endl;
    cout << "目前使用" << threadSize << "个线程" << endl;

    NetServer netServer(serverInfo,threadSize);
    netServer.StartServer();
    return 0;
}
