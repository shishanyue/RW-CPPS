//
// Created by 21553 on 2023/6/22.
//

#ifndef RW_CPS_NETCONNECTPROOFOFWORK_H
#define RW_CPS_NETCONNECTPROOFOFWORK_H
#include <cinttypes>
#include <string>
#include <ctime>
#include <string.h>

#include "utils/utils.h"

using namespace std;
class NetConnectProofOfWork {
public:
    NetConnectProofOfWork();
    uint32_t resultInt;
    uint32_t authenticateType;
    uint32_t initInt_1;
    uint32_t initInt_2;
    string outcome;
    string fixedInitial;
    uint32_t off;
    uint32_t maximumNumberOfCalculations;
    bool check(uint32_t _resultInt,
               uint32_t _authenticateType,
               string *_offIn) const;
};


#endif //RW_CPS_NETCONNECTPROOFOFWORK_H
