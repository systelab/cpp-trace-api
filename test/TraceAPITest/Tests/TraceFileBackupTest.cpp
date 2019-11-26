#include "stdafx.h"
#include "TraceFileBaseTest.h"


using namespace testing;

namespace systelab { namespace trace { namespace unit_test {

	class TraceFileBackupTest : public TraceFileBaseTest
	{
	public:
		void SetUp()
		{
			TraceFileBaseTest::SetUp();
		}

		void TearDown()
		{
			TraceFileBaseTest::TearDown();
		}

		std::vector<std::string> getSubfolders()
		{
			std::vector<std::string> folders;

			boost::filesystem::directory_iterator end_itr;
			for (boost::filesystem::directory_iterator itr(m_tracesFolderPath); itr != end_itr; ++itr)
			{
				boost::filesystem::path currentPath = itr->path();
				if (boost::filesystem::is_directory(currentPath))
				{
					std::string folderName = currentPath.filename().string();
					folders.push_back(folderName);
				}
			}

			std::sort(folders.rbegin(), folders.rend());

			return folders;
		}

		void createSubfolders(const std::vector<std::string> subfolders)
		{
			for (auto subfolder : subfolders)
			{
				boost::filesystem::create_directories(boost::filesystem::path(m_tracesFolderPath) / subfolder);
			}
		}
	};


	TEST_F(TraceFileBackupTest, testBackupCreatesLogsFolder)
	{
		TRACE_CHANNEL(m_channelName) << "First trace line before backup.";
		TRACE_CHANNEL(m_channelName) << "Second trace line before backup.";
		m_fileAgent->flush();
		m_fileAgent->backup();
		TRACE_CHANNEL(m_channelName) << "Line after backup.";
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(1, traceFileLines.size());
		EXPECT_TRUE(assertTraceLine(traceFileLines[0], m_channelName, "Line after backup."));

		auto subfolders = getSubfolders();
		ASSERT_EQ(1, subfolders.size());
		ASSERT_EQ("Logs", subfolders[0].substr(0,4));
	}

	TEST_F(TraceFileBackupTest, testBackupRemovesFoldersThatExceedMaxNumberOfArchiveFolders)
	{
		createSubfolders({ "Logs_2010_01_01", "Logs_2011_01_01", "Logs_2010_03_15", "Logs_2010_07_31", "Logs_2010_12_31" });

		TRACE_CHANNEL(m_channelName) << "Some content.";
		m_fileAgent->flush();
		m_fileAgent->backup();

		auto subfolders = getSubfolders();
		ASSERT_EQ(m_nArchivedTraceFiles, subfolders.size());
		ASSERT_EQ("Logs", subfolders[0].substr(0, 4));
		ASSERT_EQ("Logs_2011_01_01", subfolders[1]);
		ASSERT_EQ("Logs_2010_12_31", subfolders[2]);
	}

}}}

