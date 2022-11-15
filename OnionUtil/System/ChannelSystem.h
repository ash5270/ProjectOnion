#pragma once
#include "../Socket/Session.h"
#include <list>
#include <vector>

namespace onion::system
{
	class Channel
	{
	public:
		Channel();
		~Channel();

		std::list<socket::Session*>* GetChannelUsers();
	private:
		std::list<socket::Session*> m_channelUsers;
	};

	class ChannelSystem
	{
	public:
		ChannelSystem();
		~ChannelSystem();

	private:
		std::vector<Channel*> m_channels;
	};
}
