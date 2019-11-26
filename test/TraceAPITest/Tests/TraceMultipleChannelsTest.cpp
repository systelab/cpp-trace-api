#include "stdafx.h"
#include "TraceFileBaseTest.h"


using namespace testing;

namespace systelab { namespace trace { namespace unit_test {

	class TraceMultipleChannelsTest : public TraceFileBaseTest
	{
	public:
		void SetUp()
		{
			TraceFileBaseTest::SetUp();

			m_secondChannelName = "SecondChannel";
			m_secondTraceFileName = "SecondTraceFile";
			m_secondTraceFilepath = boost::filesystem::path(m_tracesFolderPath) / (m_secondTraceFileName + ".log");

			m_secondFileAgent = std::make_unique<FileAgent>(m_secondChannelName, m_secondTraceFileName, m_tracesFolderPath, m_nArchivedTraceFiles);
			m_secondFileAgent->enable(true);
		}

		void TearDown()
		{
			m_secondFileAgent.reset();

			if (boost::filesystem::exists(m_secondTraceFilepath))
			{
				boost::filesystem::remove(m_secondTraceFilepath);
			}

			TraceFileBaseTest::TearDown();
		}

		std::vector<std::string> readSecondTraceFile()
		{
			std::vector<std::string> lines;

			std::ifstream ifs(m_secondTraceFilepath.string());
			if (ifs)
			{
				std::string line;
				while (std::getline(ifs, line, '\n'))
				{
					lines.push_back(line);
				}
			}

			return lines;
		}

	protected:
		std::unique_ptr<FileAgent> m_secondFileAgent;

		std::string m_secondChannelName;
		std::string m_secondTraceFileName;
		boost::filesystem::path m_secondTraceFilepath;
	};


	TEST_F(TraceMultipleChannelsTest, testAddTracesWritesTracesToProperChannel)
	{
		TRACE_CHANNEL(m_channelName)		<< "Trace " << 1 << " for first channel.";
		TRACE_CHANNEL(m_secondChannelName)	<< "Trace " << 1 << " for second channel.";
		TRACE_CHANNEL(m_channelName)		<< "Trace " << 2 << " for first channel.";
		TRACE_CHANNEL(m_secondChannelName)	<< "Trace " << 2 << " for second channel.";

		m_fileAgent->flush();
		m_secondFileAgent->flush();

		auto firstTraceFileLines = readTraceFile();
		ASSERT_EQ(2, firstTraceFileLines.size());
		EXPECT_TRUE(assertTraceLine(firstTraceFileLines[0], m_channelName, "Trace 1 for first channel.")) << "For line 1";
		EXPECT_TRUE(assertTraceLine(firstTraceFileLines[1], m_channelName, "Trace 2 for first channel.")) << "For line 2";

		auto secondTraceFileLines = readSecondTraceFile();
		ASSERT_EQ(2, secondTraceFileLines.size());
		EXPECT_TRUE(assertTraceLine(secondTraceFileLines[0], m_secondChannelName, "Trace 1 for second channel.")) << "For line 1";
		EXPECT_TRUE(assertTraceLine(secondTraceFileLines[1], m_secondChannelName, "Trace 2 for second channel.")) << "For line 2";
	}

	TEST_F(TraceMultipleChannelsTest, testAddTraceWhenOneChannelDisabledOnlyWritesTracesOfOtherChannelIntoFile)
	{
		m_fileAgent->enable(false);

		ASSERT_FALSE(m_fileAgent->isEnabled());
		ASSERT_TRUE(m_secondFileAgent->isEnabled());

		TRACE_CHANNEL(m_channelName) << "Trace " << 1 << " for first channel.";
		TRACE_CHANNEL(m_secondChannelName) << "Trace " << 1 << " for second channel.";
		TRACE_CHANNEL(m_channelName) << "Trace " << 2 << " for first channel.";
		TRACE_CHANNEL(m_secondChannelName) << "Trace " << 2 << " for second channel.";

		m_fileAgent->flush();
		m_secondFileAgent->flush();

		ASSERT_FALSE(boost::filesystem::exists(m_traceFilepath));

		auto secondTraceFileLines = readSecondTraceFile();
		ASSERT_EQ(2, secondTraceFileLines.size());
		EXPECT_TRUE(assertTraceLine(secondTraceFileLines[0], m_secondChannelName, "Trace 1 for second channel.")) << "For line 1";
		EXPECT_TRUE(assertTraceLine(secondTraceFileLines[1], m_secondChannelName, "Trace 2 for second channel.")) << "For line 2";
	}

}}}

