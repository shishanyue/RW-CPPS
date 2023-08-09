//
// Created by 21553 on 2023/6/22.
//

#include "TypeRelay/TypeRelay.h"


void TypeRelay::TypeRelayCheck(map<Socket , GameVersionRelay> *connectionMap, GameVersionRelay *_con) {


    permissionStatus _permissionStatus = _con->connectionInfo.permission;
    PacketType packetType = _con->inputStream.packet.packetType;

    switch (_permissionStatus) {

        case permissionStatus::InitialConnection: {
            if(packetType == PacketType::PREREGISTER_INFO_RECEIVE){
                _con->sendRelayServerInfo();
                _con->connectionInfo.permission = permissionStatus::WaitCertified;
                _con->sendVerifyClientValidity();
                //_con->relayServerInternal();
                //设置cachePacket用于WaitCertified验证成功后，获取玩家name
                _con->setCachePacket(&_con->inputStream.packet);
                break;
            }
        }
        case permissionStatus::GetPlayerInfo:
            break;
        case permissionStatus::WaitCertified:{
            if(packetType == PacketType::RELAY_POW_RECEIVE){

                if(receiveVerifyClientValidity(_con,&_con->inputStream)){

                    _con->connectionInfo.permission = permissionStatus::CertifiedEnd;
                    //_con->relayServerTypeInternal();

                    if(_con->playerInfo.playerName.empty()){
                        _con->relayDirectInspection(nullptr);
                    }
                }else{
                    _con->sendVerifyClientValidity();
                }
            }else{
                _con->disconnect();
            }
            break;
        }

        case permissionStatus::CertifiedEnd: {

            if (packetType == PacketType::RELAY_118_117_RETURN) {

                _con->sendRelayServerTypeReply();
            }else{
                _con->disconnect();
            }
            break;
        }

        case permissionStatus::PlayerPermission:{
            switch (packetType) {
                 // USERS (NON-HOST) SHOULD NOT SEND RELAY PACKETS
                case PacketType::RELAY_117:
                case PacketType::RELAY_118_117_RETURN:
                case PacketType::RELAY_POW:
                case PacketType::RELAY_POW_RECEIVE:
                case PacketType::RELAY_VERSION_INFO:
                case PacketType::RELAY_BECOME_SERVER:
                case PacketType::FORWARD_CLIENT_ADD:
                case PacketType::FORWARD_CLIENT_REMOVE:
                case PacketType::PACKET_FORWARD_CLIENT_FROM:
                case PacketType::PACKET_FORWARD_CLIENT_TO:
                case PacketType::PACKET_FORWARD_CLIENT_TO_REPEATED:
                    // 防止假冒
                case PacketType::TICK:
                case PacketType::SYNC:
                case PacketType::SERVER_INFO:
                case PacketType::HEART_BEAT:
                case PacketType::START_GAME:
                case PacketType::RETURN_TO_BATTLEROOM:
                case PacketType::CHAT:
                case PacketType::KICK:
                case PacketType::PACKET_RECONNECT_TO:
                 // 每个玩家不一样, 不需要处理/缓存
                case PacketType::PASSWD_ERROR:
                case PacketType::TEAM_LIST:
                case PacketType::PREREGISTER_INFO:
                 // Nobody dealt with it
                case PacketType::PACKET_DOWNLOAD_PENDING:
                 // Refusal to Process
                case PacketType::EMPTYP_ACKAGE:
                case PacketType::NOT_RESOLVED:
                    break;
                case PacketType::REGISTER_PLAYER:{
                    _con->sendPackageToHOST(&_con->inputStream.packet);
                    //con.relayRegisterConnection(packet)
                    break;
                }
                case PacketType::CHAT_RECEIVE:{
                    _con->receiveChat(&_con->inputStream.packet);
                    break;
                }
                case PacketType::ACCEPT_START_GAME:{
                    _con->sendPackageToHOST(&_con->inputStream.packet);
                    break;
                }
                case PacketType::DISCONNECT:{
                    _con->disconnect();
                    break;
                }
                default:{
                    _con->sendPackageToHOST(&_con->inputStream.packet);
                    break;
                }
            }
            break;
        }

        case permissionStatus::HostPermission:{

            switch (packetType) {
                case PacketType::HEART_BEAT:{
                    _con->getPingData(&_con->inputStream);
                    break;
                }
                case PacketType::PACKET_FORWARD_CLIENT_TO:{
                    _con->addRelaySend(&_con->inputStream);
                    break;
                }
                case PacketType::DISCONNECT:{
                    _con->disconnect();
                    break;
                }
            }

            break;
        }

        case permissionStatus::Debug:
            break;
        case permissionStatus::Null:
            break;
    }
}

inline bool TypeRelay::receiveVerifyClientValidity(GameVersionRelay *_con, GameInputStream *inputStream) {
    if(_con->netConnectAuthenticate!= nullptr){
        uint32_t _resultInt,_authenticateType;

        _resultInt = inputStream->readInt();
        _authenticateType = inputStream->readInt();
        string _offIn = inputStream->readString();

        if(_con->netConnectAuthenticate->check(_resultInt,_authenticateType,&_offIn)){

            //delete _con->netConnectAuthenticate;
            _con->netConnectAuthenticate = nullptr;
            return true;
        }
    }
    return false;
}
