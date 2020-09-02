#include "stdafx.h"
#include "FileAgent.h"

#include "Configuration.h"

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/keywords/file_name.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time/time_clock.hpp>


using namespace boost::log;

namespace systelab { namespace trace {

	FileAgent::FileAgent(std::unique_ptr<Configuration> configuration)
		:m_configuration(std::move(configuration))
		,m_enabled(false)
	{
		std::string baseFolderPath = m_configuration->getBaseFolderPath();
		if (!boost::filesystem::exists(boost::filesystem::path(baseFolderPath)))
		{
			boost::filesystem::create_directories(baseFolderPath);
		}

		std::string channelName = m_configuration->getChannelName();
		m_activeLogFileName = boost::filesystem::path(baseFolderPath) / boost::filesystem::path(channelName + ".log");
		createSink();
	}

	FileAgent::~FileAgent()
	{
		enable(false);
		destroySink();
	}

	bool FileAgent::isEnabled() const
	{
		return m_enabled;
	}

	void FileAgent::enable(bool enable)
	{
		if (enable && !m_enabled)
		{
			boost::log::core::get()->add_sink(m_logSinkFrontend);
		}
		else if (m_enabled && !enable)
		{
			boost::log::core::get()->remove_sink(m_logSinkFrontend);
		}

		m_enabled = enable;
	}

	void FileAgent::flush()
	{
		m_logSinkFrontend->flush();
	}

	void FileAgent::rotate()
	{
		m_logSinkBackend->rotate();
	}

	void FileAgent::createSink()
	{
		m_logSinkBackend = boost::make_shared<FileAgentSinkBackend>(m_activeLogFileName, *m_configuration);
		m_logSinkFrontend = boost::make_shared<SinkFrontendType>(m_logSinkBackend);

		m_logSinkFrontend->set_formatter(
			expressions::stream
			<< expressions::format_date_time<boost::local_time::local_date_time>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f %Q")
			<< " ["
			<< expressions::attr< std::string >("Channel")
			<< "]> "
			<< expressions::smessage);

		m_logSinkFrontend->set_filter(
			expressions::has_attr< std::string >("Channel") &&
			expressions::attr<std::string>("Channel") == m_configuration->getChannelName()
		);
	}

	void FileAgent::destroySink()
	{
		m_logSinkFrontend.reset();
	}

}}