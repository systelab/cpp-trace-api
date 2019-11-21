#pragma once

#include "IFileAgent.h"
#include "FileAgentSinkBackend.h"

#include <boost/log/sinks/async_frontend.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>


namespace bl = boost::log;

namespace systelab { namespace trace {
	
	class FileAgent : public IFileAgent
	{
	public:
		FileAgent(const std::string& channel,
				  const std::string& baseTraceFilename,
				  const std::string& tracesFolderPath,
				  unsigned int nArchivedTraceFiles);
		virtual ~FileAgent();

		bool isEnabled() const override;
		void enable(bool) override;

		void flush() override;
		void backup() override;

	private:
		void createSink();
		void destroySink();

	private:
		std::string m_channel;
		std::string m_baseTraceFilename;
		std::string m_tracesFolderPath;
		unsigned int m_nArchivedTraceFiles;
		bool m_enabled;

		boost::filesystem::path m_activeLogFileName;
		typedef bl::sinks::asynchronous_sink<FileAgentSinkBackend> SinkFrontendType;
		boost::shared_ptr<FileAgentSinkBackend> m_logSinkBackend;
		boost::shared_ptr<SinkFrontendType> m_logSinkFrontend;
	};

}}

