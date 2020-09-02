#include "stdafx.h"
#include "FileAgentSinkBackend.h"

#include "Configuration.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/locks.hpp>
#include <regex>


namespace systelab { namespace trace {

	FileAgentSinkBackend::FileAgentSinkBackend(const boost::filesystem::path& logFilepath,
											   const Configuration& configuration)
		: m_logFilepath(logFilepath)
		, m_rotationBasePath(logFilepath.parent_path())
		, m_configuration(configuration)
		, m_enabled(true)
		, m_lastRecordTime(boost::none)
	{
	}

	FileAgentSinkBackend::~FileAgentSinkBackend() = default;

	void FileAgentSinkBackend::consume(boost::log::record_view const& record, string_type const& message)
	{
		boost::posix_time::ptime currentTime = boost::posix_time::second_clock::local_time();
		if (m_lastRecordTime && (currentTime.date() > (*m_lastRecordTime).date()))
		{
			rotate();
		}

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

			m_lastRecordTime = currentTime;
		}
	}

	void FileAgentSinkBackend::rotate()
	{
		boost::lock_guard<boost::mutex> guard(m_mutex);

		// Create the rotation folder
		boost::posix_time::ptime rotationTime = m_lastRecordTime ? *m_lastRecordTime : boost::posix_time::second_clock::local_time();

		std::stringstream rotationFolderStream;
		boost::posix_time::time_facet *tf = new boost::posix_time::time_facet("%Y_%m_%d");
		rotationFolderStream.imbue(std::locale(rotationFolderStream.getloc(), tf));
		rotationFolderStream << m_configuration.getRotationFoldersPrefix() << "_" << rotationTime;
		std::string rotationFolderName = rotationFolderStream.str();
		boost::filesystem::path rotationFolder(m_rotationBasePath / rotationFolderName);

		if (!boost::filesystem::exists(rotationFolder))
		{
			boost::filesystem::create_directory(rotationFolder);
		}

		// Construct the rotation filename.
		std::stringstream rotationFileStream;
		rotationFileStream << m_logFilepath.stem().string() << "_" << boost::posix_time::to_iso_string(rotationTime) << ".log";
		std::string rotationFileName = rotationFileStream.str();
		boost::filesystem::path rotationFile(rotationFolder / rotationFileName);

		// Move log file to rotation folder
		if (boost::filesystem::exists(m_logFilepath))
		{
			boost::filesystem::rename(m_logFilepath, rotationFile);
		}

		// Remove excess rotation folders
		removeExcessRotationFolders();
	}

	void FileAgentSinkBackend::removeExcessRotationFolders()
	{
		std::vector<boost::filesystem::path> rotationDayFolders;

		boost::filesystem::directory_iterator end_itr;
		for (boost::filesystem::directory_iterator itr(m_rotationBasePath); itr != end_itr; ++itr)
		{
			boost::filesystem::path currentPath = itr->path();
			if (boost::filesystem::is_directory(currentPath))
			{
				std::string folderName = currentPath.filename().string();
				std::regex folderPattern("^" + m_configuration.getRotationFoldersPrefix() + "_\\d{4}_\\d{2}_\\d{2}$");
				if (std::regex_search(folderName, folderPattern))
				{
					rotationDayFolders.push_back(currentPath);
				}
			}
		}

		std::sort(rotationDayFolders.rbegin(), rotationDayFolders.rend());

		size_t nRotationDayFolders = rotationDayFolders.size();
		for (size_t i = 0; i < nRotationDayFolders; i++)
		{
			if (i >= m_configuration.getMaxRotationDays())
			{
				boost::filesystem::remove_all(rotationDayFolders[i]);
			}
		}
	}

}}
