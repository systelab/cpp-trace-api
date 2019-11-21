#include "stdafx.h"
#include "Channel.h"

#include <sstream>


namespace systelab { namespace trace {

	Channel::Channel(const std::string& channelName)
		:m_logger(boost::log::keywords::channel = channelName)
	{
	}

	Channel::~Channel() = default;

	boost::log::sources::channel_logger<std::string>& Channel::getLogger()
	{
		return m_logger;
	}

}}
