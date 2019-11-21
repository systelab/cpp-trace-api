#include "stdafx.h"
#include "FileAgent.h"

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/keywords/file_name.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time/time_clock.hpp>


using namespace boost::log;

namespace systelab { namespace trace {

	FileAgent::FileAgent(const std::string& channel,
						 const std::string& baseTraceFilename,
						 const std::string& tracesFolderPath,
						 unsigned int nArchivedTraceFiles)
		:m_channel(channel)
		,m_baseTraceFilename(baseTraceFilename)
		,m_tracesFolderPath(tracesFolderPath)
		,m_nArchivedTraceFiles(nArchivedTraceFiles)
		,m_enabled(false)
	{
		if (!boost::filesystem::exists(boost::filesystem::path(tracesFolderPath)))
		{
			boost::filesystem::create_directories(tracesFolderPath);
		}

		m_activeLogFileName = boost::filesystem::path(tracesFolderPath) / boost::filesystem::path(baseTraceFilename + ".log");
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

	void FileAgent::backup()
	{
		m_logSinkBackend->backup();
	}

	void FileAgent::createSink()
	{
		m_logSinkBackend = boost::make_shared<FileAgentSinkBackend>(m_activeLogFileName, m_nArchivedTraceFiles);
		m_logSinkFrontend = boost::make_shared<SinkFrontendType>(m_logSinkBackend);

		m_logSinkFrontend->set_formatter(
			expressions::stream
			<< expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
			<< " ["
			<< expressions::attr< std::string >("Channel")
			<< "]> "
			<< expressions::smessage);

		m_logSinkFrontend->set_filter(
			expressions::has_attr< std::string >("Channel") &&
			expressions::attr<std::string>("Channel") == m_channel
		);
	}

	void FileAgent::destroySink()
	{
		m_logSinkFrontend.reset();
	}

}}