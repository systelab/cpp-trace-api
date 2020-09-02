#pragma once

#include <boost/optional.hpp>


namespace systelab { namespace trace {

	class UTCOffset
	{
	public:
		UTCOffset();
		virtual ~UTCOffset();

		std::string getCurrentOffset() const;

	private:
		std::string calculateCurrentOffset() const;


	private:
		static boost::optional<std::string> m_currentOffset;
	};

}}
