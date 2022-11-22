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
    virtual PacketHeader* Serialize(Stream& buffer){return nullptr;}
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

class PK_S_ANS_LOGIN : public Packet 
{
public:
    PacketID type() override {return E_S_ANS_LOGIN;}
    std::wstring     id;
    int32_t     result;

    PacketHeader* Serialize(Stream& buffer)override 
    {
        PacketHeader* header;
        buffer << header;
        header->packetId = type();
        buffer<<id;
        buffer<<result;
        return header;
    }
    void Deserialize(Stream& buffer)override 
    {
        buffer>>&id;
        buffer>>&result;
    }
};

class PK_C_REQ_LOGIN : public Packet 
{
public:
    PacketID type() override {return E_C_REQ_LOGIN;}
    std::wstring     id;
    std::wstring     password;

    PacketHeader* Serialize(Stream& buffer)override 
    {
        PacketHeader* header;
        buffer << header;
        header->packetId = type();
        buffer<<id;
        buffer<<password;
        return header;
    }
    void Deserialize(Stream& buffer)override 
    {
        buffer>>&id;
        buffer>>&password;
    }
};

class PK_S_NOTIFY_USER_REGISTER : public Packet 
{
public:
    PacketID type() override {return E_S_NOTIFY_USER_REGISTER;}
    std::wstring     id;

    PacketHeader* Serialize(Stream& buffer)override 
    {
        PacketHeader* header;
        buffer << header;
        header->packetId = type();
        buffer<<id;
        return header;
    }
    void Deserialize(Stream& buffer)override 
    {
        buffer>>&id;
    }
};

class PK_C_REQ_CHANNEL_USERINFO : public Packet 
{
public:
    PacketID type() override {return E_C_REQ_CHANNEL_USERINFO;}
    std::wstring     id;
    int32_t     channel_number;

    PacketHeader* Serialize(Stream& buffer)override 
    {
        PacketHeader* header;
        buffer << header;
        header->packetId = type();
        buffer<<id;
        buffer<<channel_number;
        return header;
    }
    void Deserialize(Stream& buffer)override 
    {
        buffer>>&id;
        buffer>>&channel_number;
    }
};

class PK_S_ANS_CHANNEL_USERINFO : public Packet 
{
public:
    PacketID type() override {return E_S_ANS_CHANNEL_USERINFO;}
    int32_t     user_count;
    std::wstring     users;
    float     pos_x;
    float     pos_y;
    float     pos_z;
    int32_t     char_code;

    PacketHeader* Serialize(Stream& buffer)override 
    {
        PacketHeader* header;
        buffer << header;
        header->packetId = type();
        buffer<<user_count;
        buffer<<users;
        buffer<<pos_x;
        buffer<<pos_y;
        buffer<<pos_z;
        buffer<<char_code;
        return header;
    }
    void Deserialize(Stream& buffer)override 
    {
        buffer>>&user_count;
        buffer>>&users;
        buffer>>&pos_x;
        buffer>>&pos_y;
        buffer>>&pos_z;
        buffer>>&char_code;
    }
};

class PK_C_NOTIFY_POSION : public Packet 
{
public:
    PacketID type() override {return E_C_NOTIFY_POSION;}
    std::wstring     id;
    float     pos_x;
    float     pos_y;
    float     pos_z;
    float     velocity_x;
    float     velocity_y;

    PacketHeader* Serialize(Stream& buffer)override 
    {
        PacketHeader* header;
        buffer << header;
        header->packetId = type();
        buffer<<id;
        buffer<<pos_x;
        buffer<<pos_y;
        buffer<<pos_z;
        buffer<<velocity_x;
        buffer<<velocity_y;
        return header;
    }
    void Deserialize(Stream& buffer)override 
    {
        buffer>>&id;
        buffer>>&pos_x;
        buffer>>&pos_y;
        buffer>>&pos_z;
        buffer>>&velocity_x;
        buffer>>&velocity_y;
    }
};


