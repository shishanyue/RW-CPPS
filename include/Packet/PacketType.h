//
// Created by 21553 on 2023/6/22.
//

#ifndef RW_CPPS_PACKETTYPE_H
#define RW_CPPS_PACKETTYPE_H
#include <cstdint>
#include <typeinfo>

//铁锈数据包类型
enum class PacketType {
    /**
     * CUSTOM PACKET
     */
    /* DEBUG */
    SERVER_DEBUG_RECEIVE = 2000,
    SERVER_DEBUG=2001,

    /* Ex */
    GET_SERVER_INFO_RECEIVE=3000,
    GET_SERVER_INFO=3001,
    UPDATA_CLASS_RECEIVE=3010,
    STATUS_RESULT=3999,

    /**
     * Game Core Packet
     */
    /* Preregister */
    PREREGISTER_INFO_RECEIVE=160,
    PREREGISTER_INFO=161,
    PASSWD_ERROR=113,
    REGISTER_PLAYER=110,

    /* Server Info */
    SERVER_INFO=106,
    TEAM_LIST=115,

    /* Heart */
    HEART_BEAT=108,
    HEART_BEAT_RESPONSE=109,

    /* Chat */
    CHAT_RECEIVE=140,
    CHAT=141,

    /* Net Status */
    PACKET_DOWNLOAD_PENDING=4,
    KICK=150,
    DISCONNECT=111,

    /* StartGame */
    START_GAME=120,
    ACCEPT_START_GAME=112,
    RETURN_TO_BATTLEROOM=122,

    /* GameStart Commands */
    TICK=10,
    GAMECOMMAND_RECEIVE=20,
    SYNCCHECKSUM_STATUS=31,
    SYNC_CHECK=30,
    SYNC=35,

    /* Relay */
    RELAY_117=117,
    RELAY_118_117_RETURN=118,
    RELAY_POW=151,
    RELAY_POW_RECEIVE=152,

    RELAY_VERSION_INFO=163,
    RELAY_BECOME_SERVER=170,
    FORWARD_CLIENT_ADD=172,
    FORWARD_CLIENT_REMOVE=173,
    PACKET_FORWARD_CLIENT_FROM=174,
    PACKET_FORWARD_CLIENT_TO=175,
    PACKET_FORWARD_CLIENT_TO_REPEATED=176,
    PACKET_RECONNECT_TO=178,


    EMPTYP_ACKAGE=0,
    NOT_RESOLVED=-1
};

class PacketTypeUtils{
public:
    /*将uint32转换为PacketType,如果不是合法值,
      返回NOT_RESOLVED(PacketType)
     */
    static PacketType ConvertToPacketType(uint32_t packetType) {
        return typeid(static_cast<PacketType>(packetType)) == typeid(uint32_t) ? PacketType::NOT_RESOLVED :static_cast<PacketType>(packetType);
    };
};
#endif //RW_CPPS_PACKETTYPE_H
