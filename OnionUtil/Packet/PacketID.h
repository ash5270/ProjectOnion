#pragma once
#include<iostream>

enum PacketID : int32_t{
    /*0*/  E_S_ANS_CHATTING = 1,
    /*1*/  E_C_REQ_CHATTING = 2,
};
struct PacketHeader
{
    int32_t size;
    PacketID packetId;
};
