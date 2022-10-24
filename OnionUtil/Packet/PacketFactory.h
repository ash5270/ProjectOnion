#pragma once
#include "PacketID.h"
#include "Packet.h"

#include "../Util/SingleTon.h"
using namespace  onion::util;
class PacketFactory : public SingleTon<PacketFactory>
{
public:
    Packet* getPacket(int32_t packetID){
     switch (packetID){
        case E_S_ANS_CHATTING: 
            return new PK_S_ANS_CHATTING();
        case E_C_REQ_CHATTING: 
            return new PK_C_REQ_CHATTING();
        default:
            return nullptr;
        }
    }
};

 