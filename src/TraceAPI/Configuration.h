#pragma once

#include <string>
#include <vector>


namespace systelab { namespace trace {
	
	class Configuration
	{
	public:
		Configuration();
		Configuration(const Configuration&);
		virtual ~Configuration();

		std::string getTracesFolderPath() const;
		std::string getBaseTraceFilename() const;
		std::vector<std::string> getAcceptedChannels() const;
		unsigned int getMaxRotationDays() const;

		void setTracesFolderPath(const std::string&);
		void setBaseTraceFilename(const std::string&);
		void setAcceptedChannels(const std::vector<std::string>&);
		void setMaxRotationDays(unsigned int);

		virtual Configuration& operator= (const Configuration&);

	private:
		std::string m_tracesFolderPath;
		std::string m_baseTraceFilename;
		std::vector<std::string> m_acceptedChannels;
		unsigned int m_maxRotationDays;
	};

}}

