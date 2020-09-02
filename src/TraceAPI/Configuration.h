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

		std::string getChannelName() const;
		std::string getBaseFolderPath() const;
		std::string getRotationFoldersPrefix() const;
		unsigned int getMaxRotationDays() const;

		void setChannelName(const std::string&);
		void setBaseFolderPath(const std::string&);
		void setRotationFoldersPrefix(const std::string&);
		void setMaxRotationDays(unsigned int);

		virtual Configuration& operator= (const Configuration&);

	private:
		std::string m_channelName;
		std::string m_baseFolderPath;
		std::string m_rotationFoldersPrefix;
		unsigned int m_maxRotationDays;
	};

}}

