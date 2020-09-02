#include "stdafx.h"
#include "TraceFileBaseTest.h"


using namespace testing;

namespace systelab { namespace trace { namespace unit_test {

	class TraceFileRotationTest : public TraceFileBaseTest
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

		std::vector<std::string> getRotationSubfolders()
		{
			std::vector<std::string> folders;

			boost::filesystem::directory_iterator end_itr;
			for (boost::filesystem::directory_iterator itr(m_baseFolderPath); itr != end_itr; ++itr)
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

		void createRotationSubfolders(const std::vector<std::string> subfolders)
		{
			for (auto subfolder : subfolders)
			{
				boost::filesystem::create_directories(boost::filesystem::path(m_baseFolderPath) / subfolder);
			}
		}
	};


	TEST_F(TraceFileRotationTest, testBackupCreatesLogsFolder)
	{
		TRACE_CHANNEL(m_channelName) << "First trace line before backup.";
		TRACE_CHANNEL(m_channelName) << "Second trace line before backup.";
		m_fileAgent->flush();
		m_fileAgent->rotate();
		TRACE_CHANNEL(m_channelName) << "Line after backup.";
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(1, traceFileLines.size());
		EXPECT_TRUE(assertTraceLine(traceFileLines[0], m_channelName, "Line after backup."));

		auto subfolders = getRotationSubfolders();
		ASSERT_EQ(1, subfolders.size());
		ASSERT_EQ("RotationPrefix", subfolders[0].substr(0,14));
	}

	TEST_F(TraceFileRotationTest, testBackupRemovesFoldersThatExceedMaxNumberOfArchiveFolders)
	{
		createRotationSubfolders({ "RotationPrefix_2010_01_01", "RotationPrefix_2011_01_01", "RotationPrefix_2010_03_15",
								   "RotationPrefix_2010_07_31", "RotationPrefix_2010_12_31" });

		TRACE_CHANNEL(m_channelName) << "Some content.";
		m_fileAgent->flush();
		m_fileAgent->rotate();

		auto subfolders = getRotationSubfolders();
		ASSERT_EQ(m_maxRotationDays, subfolders.size());
		ASSERT_EQ("RotationPrefix", subfolders[0].substr(0, 14));
		ASSERT_EQ("RotationPrefix_2011_01_01", subfolders[1]);
		ASSERT_EQ("RotationPrefix_2010_12_31", subfolders[2]);
	}

}}}

