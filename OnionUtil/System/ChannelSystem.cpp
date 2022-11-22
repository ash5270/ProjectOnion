#include "ChannelSystem.h"
#include"../GameObject/GameObject.h"
Channel::Channel()
{
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
}

ChannelSystem::ChannelSystem()
{
}

ChannelSystem::~ChannelSystem()
{
}
