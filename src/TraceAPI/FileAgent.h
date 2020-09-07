#pragma once

#include "IFileAgent.h"
#include "FileAgentSinkBackend.h"

#include <boost/log/sinks/async_frontend.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>


namespace bl = boost::log;

BOOST_LOG_ATTRIBUTE_KEYWORD(channel_attr, "Channel", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity_attr, "Severity", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)

namespace systelab { namespace trace {

	class Configuration;

	class FileAgent : public IFileAgent
	{
	public:
		FileAgent(std::unique_ptr<Configuration>);
		virtual ~FileAgent();

		bool isEnabled() const override;
		void enable(bool) override;

		void flush() override;
		void rotate() override;

	private:
		void createSink();
		void destroySink();

		bool filterRecord(const bl::value_ref<std::string, tag::channel_attr>&,
						  const bl::value_ref<std::string, tag::severity_attr>&);
		bool filterRecordBySeverity(const bl::value_ref<std::string, tag::severity_attr>&);

	private:
		std::unique_ptr<Configuration> m_configuration;

		bool m_enabled;
		boost::filesystem::path m_activeLogFileName;
		typedef bl::sinks::asynchronous_sink<FileAgentSinkBackend> SinkFrontendType;
		boost::shared_ptr<FileAgentSinkBackend> m_logSinkBackend;
		boost::shared_ptr<SinkFrontendType> m_logSinkFrontend;
	};

}}

