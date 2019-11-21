#include "stdafx.h"
#include "ChannelMgr.h"

#include <boost/log/attributes.hpp>


namespace systelab { namespace trace {

	ChannelMgr::ChannelMgr()
	{
		boost::log::core::get()->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
	};

	ChannelMgr::~ChannelMgr() = default;

	ChannelMgr& ChannelMgr::get()
	{
		static ChannelMgr instance;
		return instance;
	}

	Channel& ChannelMgr::name(const std::string& channelName) const
	{
		auto it = m_channels.find(channelName);
		if (it != m_channels.end())
		{
			return *(it->second);
		}
		else
		{
			auto channel = std::make_unique<Channel>(channelName);
			auto& channelRef = *channel;
			m_channels.insert( std::make_pair(channelName, std::move(channel)) );
			return channelRef;
		}
	}

}}
