#pragma once

#include "ITraceAgent.h"
#include "TraceAgentSinkBackend.h"

#include <boost/log/sinks/async_frontend.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>


namespace bl = boost::log;

namespace systelab { namespace trace {
	
	class TraceAgent : public ITraceAgent
	{
	public:
		TraceAgent(const std::string& channel,
				   const std::string& baseTraceFilename,
				   const std::string& tracesFolderPath,
				   unsigned int nArchivedTraceFiles);
		virtual ~TraceAgent();

		bool isEnabled() const;
		void enable(bool);

		void backup();

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
		typedef bl::sinks::asynchronous_sink<TraceAgentSinkBackend> SinkFrontendType;
		boost::shared_ptr<TraceAgentSinkBackend> m_logSinkBackend;
		boost::shared_ptr<SinkFrontendType> m_logSinkFrontend;
	};

}}

