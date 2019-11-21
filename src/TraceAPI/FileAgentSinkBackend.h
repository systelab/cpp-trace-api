#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/thread/mutex.hpp>


namespace sinks = boost::log::sinks;

namespace systelab { namespace trace {

	class FileAgentSinkBackend : public sinks::text_file_backend
	{

	public:
		FileAgentSinkBackend(const boost::filesystem::path& m_logFilepath,
							 unsigned int maxArchiveFolders);
		virtual ~FileAgentSinkBackend() = default;

		bool isEnabled() const;
		void setEnabled(bool enabled);

		void consume(boost::log::record_view const& rec, string_type const& command_line);
		void backup();

	private:
		void removeExcessBackupFolders();

	private:
		boost::filesystem::path m_logFilepath;
		boost::filesystem::path m_backupBasePath;
		unsigned int m_maxArchiveFolders;
		bool m_enabled;
		boost::mutex m_mutex;
	};

}}

