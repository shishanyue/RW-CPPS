cmake_minimum_required(VERSION 3.19)
project(RW_CPPS)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(src/utils utils)
aux_source_directory(src/DataStream DataStream)
aux_source_directory(src/NetServer NetServer)
aux_source_directory(src/NetServer/protocol/realize NetServer)
aux_source_directory(src/NetServer/code/tcp NetServer)
aux_source_directory(src/NetServer/code/udp NetServer)
aux_source_directory(src/NetServer/code NetServer)
aux_source_directory(src/NetServer/global NetServer)
aux_source_directory(src/Packet Packet)
aux_source_directory(src/Player Player)
aux_source_directory(src/TypeRelay TypeRelay)
aux_source_directory(src/Cache Cache)
aux_source_directory(src/cppsHttp cppsHttp)
add_executable(RW_CPPS main.cpp ${utils} ${DataStream}
        ${NetServer} ${Packet} ${Player} ${TypeRelay} ${Cache} ${cppsHttp})

set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")
# 检测操作系统
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    #target_compile_definitions(rwServer PUBLIC "IS_LINUX")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    set(EXTRA_LIBS ${EXTRA_LIBS} ws2_32 Mswsock)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    target_link_libraries(RW_CPPS ${EXTRA_LIBS})
endif()