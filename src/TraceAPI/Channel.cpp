#include "stdafx.h"
#include "Channel.h"

#include "UTCOffset.h"

#include <boost/log/attributes/constant.hpp>


namespace systelab { namespace trace {

	Channel::Channel(const std::string& channelName)
		:m_channelName(channelName)
		,m_logger(boost::log::keywords::channel = channelName)
		,m_severityLoggers()
	{
		m_logger.add_attribute("UTCOffset", boost::log::attributes::constant<std::string>(UTCOffset().getCurrentOffset()));
	}

	Channel::~Channel() = default;

	boost::log::sources::channel_logger<std::string>& Channel::getLogger()
	{
		return m_logger;
	}

	boost::log::sources::channel_logger<std::string>& Channel::getSeverityLogger(const std::string& severityName)
	{
		auto itr = m_severityLoggers.find(severityName);
		if (itr != m_severityLoggers.end())
		{
			return itr->second;
		}
		else
		{
			m_severityLoggers.insert(std::make_pair(severityName, boost::log::sources::channel_logger<std::string>(boost::log::keywords::channel = m_channelName)));
			itr = m_severityLoggers.find(severityName);
			itr->second.add_attribute("Severity", boost::log::attributes::constant<std::string>(severityName));
			itr->second.add_attribute("UTCOffset", boost::log::attributes::constant<std::string>(UTCOffset().getCurrentOffset()));
			return itr->second;
		}
	}

}}
