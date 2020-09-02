#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/optional.hpp>


namespace sinks = boost::log::sinks;

namespace systelab { namespace trace {

	class Configuration;

	class FileAgentSinkBackend : public sinks::text_file_backend
	{
	public:
		FileAgentSinkBackend(const boost::filesystem::path&, const Configuration&);
		virtual ~FileAgentSinkBackend();

		void consume(boost::log::record_view const& rec, string_type const& command_line);
		void rotate();

	private:
		void removeExcessRotationFolders();

	private:
		boost::filesystem::path m_logFilepath;
		boost::filesystem::path m_rotationBasePath;
		const Configuration& m_configuration;

		bool m_enabled;
		boost::mutex m_mutex;
		boost::optional<boost::posix_time::ptime> m_lastRecordTime;
	};

}}

