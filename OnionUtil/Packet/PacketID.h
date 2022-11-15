#pragma once
#include<iostream>

enum PacketID : int32_t{
    /*0*/  E_S_ANS_CHATTING = 0,
    /*1*/  E_C_REQ_CHATTING = 1,
    /*2*/  E_S_ANS_LOGIN = 2,
    /*3*/  E_C_REQ_LOGIN = 3,
    /*4*/  E_S_NOTIFY_USER_REGISTER = 4,
    /*5*/  E_C_REQ_CHANNEL_USERINFO = 5,
    /*6*/  E_S_ANS_CHANNEL_USERINFO = 6,
    /*7*/  E_C_NOTIFY_POSION = 7,
};
struct PacketHeader
{
    int32_t size;
    PacketID packetId;
};
