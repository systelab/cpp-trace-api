#pragma once

#include <string>
#include <iostream>

#include <boost/log/sources/channel_logger.hpp>
#include <boost/log/common.hpp>


namespace systelab { namespace trace {

	class Channel
	{
	public:
		Channel(const std::string& channelName);
		virtual ~Channel();

		boost::log::sources::channel_logger<std::string>& getLogger();
		boost::log::sources::channel_logger<std::string>& getSeverityLogger(const std::string& severity);

	protected:
		std::string m_channelName;
		boost::log::sources::channel_logger<std::string> m_logger;
		std::map<std::string, boost::log::sources::channel_logger<std::string>> m_severityLoggers;
	};

}}

