#pragma once
#include<iostream>

enum PacketID : int32_t{
    /*0*/  E_S_ANS_CHATTING = 0,
    /*1*/  E_C_REQ_CHATTING = 1,
};
struct PacketHeader
{
    int32_t size;
    PacketID packetId;
};
