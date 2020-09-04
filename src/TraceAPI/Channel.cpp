#include "stdafx.h"
#include "Channel.h"

#include "UTCOffset.h"

#include <boost/log/attributes/constant.hpp>


namespace systelab { namespace trace {

	Channel::Channel(const std::string& channelName)
		:m_channelName(channelName)
		,m_loggersMap()
	{
	}

	Channel::~Channel() = default;

	boost::log::sources::channel_logger<std::string>& Channel::getLogger(const std::string& severity, const std::string& tag)
	{
		std::string mapKey = severity + "<>" + tag;
		auto itr = m_loggersMap.find(mapKey);
		if (itr != m_loggersMap.end())
		{
			return itr->second;
		}
		else
		{
			m_loggersMap.insert(std::make_pair(mapKey, boost::log::sources::channel_logger<std::string>(boost::log::keywords::channel = m_channelName)));
			itr = m_loggersMap.find(mapKey);

			itr->second.add_attribute("UTCOffset", boost::log::attributes::constant<std::string>(UTCOffset().getCurrentOffset()));

			if (severity.size() > 0)
			{
				itr->second.add_attribute("Severity", boost::log::attributes::constant<std::string>(severity));
			}

			if (tag.size() > 0)
			{
				itr->second.add_attribute("Tag", boost::log::attributes::constant<std::string>(tag));
			}

			return itr->second;
		}
	}

}}
