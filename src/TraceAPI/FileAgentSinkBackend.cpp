#include "stdafx.h"
#include "FileAgentSinkBackend.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/locks.hpp>
#include <regex>


namespace systelab { namespace trace {

	FileAgentSinkBackend::FileAgentSinkBackend(const boost::filesystem::path& logFilepath,
											   unsigned int maxArchiveFolders)
		:m_logFilepath(logFilepath)
		,m_backupBasePath(logFilepath.parent_path())
		,m_maxArchiveFolders(maxArchiveFolders)
		,m_enabled(true)
	{
	}

	FileAgentSinkBackend::~FileAgentSinkBackend() = default;

	void FileAgentSinkBackend::consume(boost::log::record_view const&, string_type const& message)
	{
		boost::lock_guard<boost::mutex> guard(m_mutex);
		if (m_enabled)
		{
			std::ofstream logFileStream;
			logFileStream.open(m_logFilepath.string(), std::ios::out | std::ios::app);
			if (!logFileStream.fail())
			{
				logFileStream << message << std::endl;
				logFileStream.close();
			}
		}
	}

	void FileAgentSinkBackend::backup()
	{
		boost::lock_guard<boost::mutex> guard(m_mutex);

		// Create the backup folder
		boost::posix_time::ptime currentTime = boost::posix_time::second_clock::universal_time();

		std::stringstream backupFolderStream;
		boost::posix_time::time_facet *tf = new boost::posix_time::time_facet("%Y_%m_%d");
		backupFolderStream.imbue(std::locale(backupFolderStream.getloc(), tf));
		backupFolderStream << "Logs_" << currentTime;
		std::string backupFolderName = backupFolderStream.str();
		boost::filesystem::path backupFolder(m_backupBasePath / backupFolderName);

		if (!boost::filesystem::exists(backupFolder))
		{
			boost::filesystem::create_directory(backupFolder);
		}

		// Construct the backup filename.
		std::stringstream backupFileStream;
		backupFileStream << m_logFilepath.stem().string() << "_" << boost::posix_time::to_iso_string(currentTime) << ".log";
		std::string backupFileName = backupFileStream.str();
		boost::filesystem::path backupFile(backupFolder / backupFileName);

		// Move log file to backup folder
		if (boost::filesystem::exists(m_logFilepath))
		{
			boost::filesystem::rename(m_logFilepath, backupFile);
		}

		// Remove excess backup folders
		removeExcessBackupFolders();
	}

	void FileAgentSinkBackend::removeExcessBackupFolders()
	{
		std::vector<boost::filesystem::path> logFolders;

		boost::filesystem::directory_iterator end_itr;
		for (boost::filesystem::directory_iterator itr(m_backupBasePath); itr != end_itr; ++itr)
		{
			boost::filesystem::path currentPath = itr->path();
			if (boost::filesystem::is_directory(currentPath))
			{
				std::string folderName = currentPath.filename().string();
				std::regex folderPattern("^Logs_\\d{4}_\\d{2}_\\d{2}$");
				if (std::regex_search(folderName, folderPattern))
				{
					logFolders.push_back(currentPath);
				}
			}
		}

		std::sort(logFolders.rbegin(), logFolders.rend());

		size_t nLogFolders = logFolders.size();
		for (size_t i = 0; i < nLogFolders; i++)
		{
			if (i >= m_maxArchiveFolders)
			{
				boost::filesystem::remove_all(logFolders[i]);
			}
		}
	}

}}
