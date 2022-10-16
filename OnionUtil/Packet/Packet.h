#pragma once
#include<iostream>

#include "../System/Buffer.h"
#include "PacketID.h"

using namespace onion::system;

class Packet
{
public:
    virtual PacketID type() = 0;
    virtual void Serialize(Buffer& buffer) { buffer << (int32_t)this->type(); }
    virtual void Deserialize(Buffer& buffer) {}
};

class PK_S_ANS_CHATTING : public Packet 
{
public:
    PacketID type() override {return E_S_ANS_CHATTING;}
    std::wstring     id;
    std::wstring     msg;

    void Serialize(Buffer& buffer)override 
    {
        buffer<<(int32_t)this->type();
        buffer<<id;
        buffer<<msg;
    }
    void Deserialize(Buffer& buffer)override 
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

    void Serialize(Buffer& buffer)override 
    {
        buffer<<(int32_t)this->type();
        buffer<<id;
        buffer<<msg;
    }
    void Deserialize(Buffer& buffer)override 
    {
        buffer>>&id;
        buffer>>&msg;
    }
};


