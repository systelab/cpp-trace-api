#include "stdafx.h"
#include "Configuration.h"


namespace systelab { namespace trace {

	Configuration::Configuration()
		: m_channelName("Log")
		, m_baseFolderPath(".")
		, m_rotationFoldersPrefix("Logs")
		, m_maxRotationDays(5)
	{
	}

	Configuration::Configuration(const Configuration& other)
		: m_channelName(other.m_channelName)
		, m_baseFolderPath(other.m_baseFolderPath)
		, m_rotationFoldersPrefix(other.m_rotationFoldersPrefix)
		, m_maxRotationDays(other.m_maxRotationDays)
	{
	}

	Configuration::~Configuration() = default;

	std::string Configuration::getChannelName() const
	{
		return m_channelName;
	}

	std::string Configuration::getBaseFolderPath() const
	{
		return m_baseFolderPath;
	}

	std::string Configuration::getRotationFoldersPrefix() const
	{
		return m_rotationFoldersPrefix;
	}

	unsigned int Configuration::getMaxRotationDays() const
	{
		return m_maxRotationDays;
	}

	void Configuration::setChannelName(const std::string& channelName)
	{
		m_channelName = channelName;
	}

	void Configuration::setBaseFolderPath(const std::string& baseFolderPath)
	{
		m_baseFolderPath = baseFolderPath;
	}

	void Configuration::setRotationFoldersPrefix(const std::string& rotationFoldersPrefix)
	{
		m_rotationFoldersPrefix = rotationFoldersPrefix;
	}

	void Configuration::setMaxRotationDays(unsigned int maxRotationDays)
	{
		m_maxRotationDays = maxRotationDays;
	}

	Configuration& Configuration::operator= (const Configuration& other)
	{
		m_channelName = other.m_channelName;
		m_baseFolderPath = other.m_baseFolderPath;
		m_rotationFoldersPrefix = other.m_rotationFoldersPrefix;
		m_maxRotationDays = other.m_maxRotationDays;

		return *this;
	}
}}