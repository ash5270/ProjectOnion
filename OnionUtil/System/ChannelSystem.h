#pragma once
#include "../Socket/Session.h"
#include <vector>
#include <unordered_map>
#include"../GameObject/GameObject.h"
#include"../GameObject/WorldMap.h"

namespace onion::system
{
	using object::GameObject;
	class Channel
	{
	public:
		Channel();
		~Channel();
		const std::unordered_map<std::wstring, socket::Session*>& GetUsersSession() const;
		const std::unordered_map<std::wstring, object::GameObject*>& GetPlayerObject() const;
		void AddPlayer(const std::wstring& userID, object::GameObject* gameobject);
		void AddPlayerSession(const std::wstring& userId, socket::Session* session);
		object::WorldMap& GetWorldMap()
		{
			return *m_world;
		}
	private:
		std::unordered_map<std::wstring, socket::Session*> m_channelUsers;
		std::unordered_map<std::wstring, object::GameObject*> m_gameObjects;
		object::WorldMap* m_world;
	};

	class ChannelSystem
	{
	public:
		ChannelSystem();
		~ChannelSystem();

	private:
	private:
		std::vector<Channel*> m_channels;
	};
}
