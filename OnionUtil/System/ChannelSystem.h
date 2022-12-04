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
		const std::unordered_map<size_t, socket::Session*>& GetUsersSession() const;
		const std::unordered_map<size_t, object::GameObject*>& GetPlayerObject() const;
		void AddPlayer(const size_t& userID, object::GameObject* gameobject);
		void AddPlayerSession(const size_t& userId, socket::Session* session);
		object::WorldMap& GetWorldMap()
		{
			return *m_world;
		}
	private:
		std::unordered_map<size_t, socket::Session*> m_channelUsers;
		std::unordered_map<size_t, object::GameObject*> m_gameObjects;
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
