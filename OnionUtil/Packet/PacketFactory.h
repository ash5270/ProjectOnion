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
        case E_S_ANS_LOGIN: 
            return new PK_S_ANS_LOGIN();
        case E_C_REQ_LOGIN: 
            return new PK_C_REQ_LOGIN();
        case E_S_NOTIFY_USER_REGISTER: 
            return new PK_S_NOTIFY_USER_REGISTER();
        case E_C_REQ_CHANNEL_USERINFO: 
            return new PK_C_REQ_CHANNEL_USERINFO();
        case E_S_ANS_CHANNEL_USERINFO: 
            return new PK_S_ANS_CHANNEL_USERINFO();
        case E_C_NOTIFY_POSION: 
            return new PK_C_NOTIFY_POSION();
        case E_S_ANS_PING: 
            return new PK_S_ANS_PING();
        case E_C_REQ_PING: 
            return new PK_C_REQ_PING();
        default:
            return nullptr;
        }
    }
};

