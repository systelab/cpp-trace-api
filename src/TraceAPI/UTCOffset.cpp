#include "stdafx.h"
#include "UTCOffset.h"

#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <sstream>


namespace systelab { namespace trace {

	boost::optional<std::string> UTCOffset::m_currentOffset = boost::none;

	UTCOffset::UTCOffset() = default;
	UTCOffset::~UTCOffset() = default;

	std::string UTCOffset::getCurrentOffset() const
	{
		if (!m_currentOffset)
		{
			m_currentOffset = calculateCurrentOffset();
		}

		return *m_currentOffset;
	}

	std::string UTCOffset::calculateCurrentOffset() const
	{
		const boost::posix_time::ptime utcTime = boost::posix_time::second_clock::universal_time();
		const boost::posix_time::ptime localTime = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(utcTime);
		const boost::posix_time::time_duration td = localTime - utcTime;

		const char fillChar = '0';
		const char timeSeparator = ':';

		std::stringstream out;
		out << (td.is_negative() ? '-' : '+');
		out << std::setw(2) << std::setfill(fillChar) << boost::date_time::absolute_value(td.hours());
		out << timeSeparator;
		out << std::setw(2) << std::setfill(fillChar) << boost::date_time::absolute_value(td.minutes());

		return out.str();
	}
}}
