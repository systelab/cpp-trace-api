#pragma once

#include <string>
#include <iostream>
#include <map>

#include <boost/log/sources/channel_logger.hpp>
#include <boost/log/common.hpp>


namespace systelab { namespace trace {

	class Channel
	{
	public:
		Channel(const std::string& channelName);
		virtual ~Channel();

		boost::log::sources::channel_logger<std::string>& getLogger(const std::string& severity, const std::string& tag);

	protected:
		std::string m_channelName;
		std::map<std::string, boost::log::sources::channel_logger<std::string>> m_loggersMap; // Map key: Serverity<>Tag
	};

}}

