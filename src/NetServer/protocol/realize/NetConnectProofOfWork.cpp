//
// Created by 21553 on 2023/6/22.
//

#include "NetServer/protocol/realize/NetConnectProofOfWork.h"

NetConnectProofOfWork::NetConnectProofOfWork() {
    RandomUtils utils;
    resultInt= time(nullptr);
    authenticateType = utils.randomInt32(0,6);
    authenticateType = authenticateType == 2 ? 5 : authenticateType;

    initInt_1 = authenticateType == 0||(authenticateType>=2&&authenticateType<=4) || authenticateType==6?utils.randomInt32(0,INT32_MAX):0;
    initInt_2 = authenticateType == 1||(authenticateType>=2&&authenticateType<=4)?utils.randomInt32(0,INT32_MAX):0;

    if(authenticateType>=3&&authenticateType<=4){
        string hex = to_string(initInt_1)+'|'+ to_string(initInt_2);
        picosha2::hash256_hex_string(hex,outcome);
        outcome.erase(14,outcome.size());
        transform(outcome.begin(),outcome.end(),outcome.begin(),::toupper);

        fixedInitial="";
        off=0;
        maximumNumberOfCalculations=0;
    } else if(authenticateType>=5&&authenticateType<=6){
        fixedInitial=utils.randomBigLetter(4);

        if(authenticateType==6){
            fixedInitial += to_string(initInt_1);
        }
        off=utils.randomInt32(0,10);
        maximumNumberOfCalculations = utils.randomInt32(0,10000000);

        string hex = fixedInitial + to_string(off);
        picosha2::hash256_hex_string(hex,outcome);
        outcome.erase(14,outcome.size());
        transform(outcome.begin(),outcome.end(),outcome.begin(),::toupper);
    } else{
        outcome="";
        fixedInitial="";
        off=0;
        maximumNumberOfCalculations=0;
    }
}

bool NetConnectProofOfWork::check(uint32_t _resultInt, uint32_t _authenticateType, string *_offIn) const {
    if(this->resultInt != _resultInt || this->authenticateType != _authenticateType){
        return false;
    }

    switch (_authenticateType) {
        case 0:return to_string(this->initInt_1) == *_offIn;
        case 1:return to_string(this->initInt_2) == *_offIn;
        case 2:return true;//TODO Game.connectKeyLast(initInt_1) == offIn
        case 3:
        case 4:return this->outcome == *_offIn;
        case 5:
        case 6:return this->off == (uint32_t)*_offIn->data();
        default:break;
    }

    return true;
}
