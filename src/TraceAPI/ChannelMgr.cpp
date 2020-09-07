#include "stdafx.h"
#include "ChannelMgr.h"

#include <boost/log/attributes.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>


namespace systelab { namespace trace {

	ChannelMgr::ChannelMgr()
	{
		boost::log::core::get()->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
		createDummySink();
	};

	ChannelMgr::~ChannelMgr()
	{
		boost::log::core::get()->remove_all_sinks();
	}

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

	void ChannelMgr::createDummySink()
	{
		// Dummy sink created just to avoid logging to console when no sink enabled
		namespace bl = boost::log;
		boost::shared_ptr<bl::sinks::text_ostream_backend> dummyBackend = boost::make_shared<bl::sinks::text_ostream_backend>();
		typedef bl::sinks::asynchronous_sink<bl::sinks::text_ostream_backend> sink_t;
		boost::shared_ptr<sink_t> sink(new sink_t(dummyBackend));
		bl::core::get()->add_sink(sink);
	}

}}
