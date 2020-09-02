#include "stdafx.h"
#include "Configuration.h"


namespace systelab { namespace trace {

	Configuration::Configuration()
		: m_tracesFolderPath("")
		, m_baseTraceFilename("")
		, m_acceptedChannels({})
		, m_maxRotationDays(5)
	{
	}

	Configuration::Configuration(const Configuration& other)
		: m_tracesFolderPath(other.m_tracesFolderPath)
		, m_baseTraceFilename(other.m_baseTraceFilename)
		, m_acceptedChannels(other.m_acceptedChannels)
		, m_maxRotationDays(other.m_maxRotationDays)
	{
	}

	Configuration::~Configuration() = default;

	std::string Configuration::getTracesFolderPath() const
	{
		return m_tracesFolderPath;
	}

	std::string Configuration::getBaseTraceFilename() const
	{
		return m_baseTraceFilename;
	}

	std::vector<std::string> Configuration::getAcceptedChannels() const
	{
		return m_acceptedChannels;
	}

	unsigned int Configuration::getMaxRotationDays() const
	{
		return m_maxRotationDays;
	}

	void Configuration::setTracesFolderPath(const std::string& tracesFolderPath)
	{
		m_tracesFolderPath = tracesFolderPath;
	}

	void Configuration::setBaseTraceFilename(const std::string& baseTraceFilename)
	{
		m_baseTraceFilename = baseTraceFilename;
	}

	void Configuration::setAcceptedChannels(const std::vector<std::string>& acceptedChannels)
	{
		m_acceptedChannels = acceptedChannels;
	}

	void Configuration::setMaxRotationDays(unsigned int maxRotationDays)
	{
		m_maxRotationDays = maxRotationDays;
	}

	Configuration& Configuration::operator= (const Configuration& other)
	{
		m_tracesFolderPath = other.m_tracesFolderPath;
		m_baseTraceFilename = other.m_baseTraceFilename;
		m_acceptedChannels = other.m_acceptedChannels;
		m_maxRotationDays = other.m_maxRotationDays;

		return *this;
	}
}}