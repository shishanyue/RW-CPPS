//
// Created by 21553 on 2023/6/22.
//

#ifndef RWSERVER_TYPERELAY_H
#define RWSERVER_TYPERELAY_H

#include "DataStream/GameInputStream.h"
#include "DataStream/GameOutputStream.h"
#include "Player/GameVersionRelay.h"
#include <map>


static string msgs;
class TypeRelay {
public:
    static void TypeRelayCheck(map<Socket , GameVersionRelay> *connectionMap,GameVersionRelay *_con);
    static bool receiveVerifyClientValidity(GameVersionRelay *_con,GameInputStream *inputStream);
};


#endif //RWSERVER_TYPERELAY_H
