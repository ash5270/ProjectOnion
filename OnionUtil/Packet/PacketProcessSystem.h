///packet 처리
///session에서 받은 packet을 모아서 처리
///
#pragma once
#include <thread>
#include "../System/safe_queue.h"
#include <unordered_map>
#include "../Socket/Session.h"
#include "LoginProcess.h"
#include <functional>

using namespace  onion;

class PacketObject;
class Packet;
class PacketProcessSystem
{
public:
	//생성자
	PacketProcessSystem();
	~PacketProcessSystem();

	//packet queue에 넣기
	void PushPacket(PacketObject* packetObject)
	{
		m_packetQueue.push(packetObject);
	}

private:
	std::thread m_thread;
	bool m_isUpdate;
	system::safe_queue<PacketObject*> m_packetQueue;
	std::unordered_map<int32_t, std::function<void(socket::Session*, Packet*)>> m_packetTable;

private:
	void Process(PacketObject* packetObject);
	void Update();

public:
	void RegisterPacketProcess(PacketID id ,std::function<void(socket::Session*,Packet*)> func)
	{
		m_packetTable.insert(std::make_pair(id, func));
	}
	void Start();
	void Stop();
};
