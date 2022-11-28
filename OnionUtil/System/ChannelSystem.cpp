#include "ChannelSystem.h"
#include"../GameObject/GameObject.h"
Channel::Channel()
{
	object::math::Rect rect = { object::math::Vector2(-105,105),object::math::Vector2(105,-105) };
	m_world = new object::WorldMap(rect);
}

Channel::~Channel()
{
}

const std::unordered_map<std::wstring, onion::socket::Session*>& Channel::GetUsersSession() const
{
	return m_channelUsers;
}

const std::unordered_map<std::wstring, onion::object::GameObject*>& Channel::GetPlayerObject() const
{
	return m_gameObjects;
}

void Channel::AddPlayer(const std::wstring& userID, object::GameObject* gameobject)
{
	if(userID==L""||gameobject==nullptr)
		return;
	auto make= std::make_pair(userID, gameobject);
	m_gameObjects.insert(make);
	m_world->AddGameObject(gameobject);
}

void Channel::AddPlayerSession(const std::wstring& userId,  socket::Session* session)
{
	auto make = std::make_pair(userId, session);
	m_channelUsers.insert(make);
}

ChannelSystem::ChannelSystem()
{
}

ChannelSystem::~ChannelSystem()
{
}
