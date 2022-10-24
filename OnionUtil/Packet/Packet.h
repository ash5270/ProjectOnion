#pragma once
#include<iostream>

#include "../System/Stream.h"
#include "PacketID.h"

using namespace onion::system;

class Packet
{
public:
    virtual PacketID type() =0;
     /**
     * \brief 패킷시리얼라이즈, 패킷 시리얼 라이즈 해주고
     * \param buffer buffer
     * \return PacketHeader Pointer
     */
    virtual PacketHeader* Serialize(Stream& buffer) { return nullptr; }
    virtual void Deserialize(Stream& buffer){}
};

class PK_S_ANS_CHATTING : public Packet 
{
public:
    PacketID type() override {return E_S_ANS_CHATTING;}
    std::wstring     id;
    std::wstring     msg;

	PacketHeader* Serialize(Stream& buffer)override 
    {
        PacketHeader* header;
        buffer << header;
        header->packetId = type();
        buffer<<id;
        buffer<<msg;
        return header;
    }
	void Deserialize(Stream& buffer)override
    {
        buffer>>&id;
        buffer>>&msg;
    }
};

class PK_C_REQ_CHATTING : public Packet 
{
public:
    PacketID type() override {return E_C_REQ_CHATTING;}
    std::wstring     id;
    std::wstring     msg;
    int count = 0;

    PacketHeader* Serialize(Stream& buffer)override 
    {
        PacketHeader* header = nullptr;
        buffer << header;
        header->packetId = type();
        buffer<<id;
        buffer<<msg;
        buffer << count;
        return header;
    }
    void Deserialize(Stream& buffer)override 
    {
        buffer>>&id;
        buffer>>&msg;
        buffer >> &count;
    }
};


