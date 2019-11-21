#pragma once

#include <string>
#include <iostream>

#include <boost/log/sources/channel_logger.hpp>
#include <boost/log/common.hpp>


namespace systelab { namespace trace {

	class Channel
	{
	public:
		Channel(const std::string&);
		virtual ~Channel();

		boost::log::sources::channel_logger<std::string>& getLogger();

	protected:
		boost::log::sources::channel_logger<std::string> m_logger;
	};

}}

