#include "stdafx.h"
#include "TraceFileBaseTest.h"


using namespace testing;

namespace systelab { namespace trace { namespace unit_test {

	class TraceFileContentTest : public TraceFileBaseTest
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
	};


	TEST_F(TraceFileContentTest, testAddTraceCreatesFile)
	{
		TRACE_CHANNEL_SEVERITY(m_channelName, "INFO") << "This is trace line " << 1 << " entry.";
		TRACE_CHANNEL_SEVERITY(m_channelName, "DEBUG") << "This is trace line " << 2 << " entry.";
		TRACE_CHANNEL_SEVERITY(m_channelName, "ERROR") << "This is trace line " << 3 << " entry.";
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(3, traceFileLines.size());
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[0], "INFO",  "This is trace line 1 entry.")) << "For line 1";
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[1], "DEBUG", "This is trace line 2 entry.")) << "For line 2";
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[2], "ERROR", "This is trace line 3 entry.")) << "For line 3";
	}

	TEST_F(TraceFileContentTest, testAddTraceWhenDisabledDoesNotWriteIntoFile)
	{
		m_fileAgent->enable(false);
		EXPECT_FALSE(m_fileAgent->isEnabled());
		TRACE_CHANNEL(m_channelName) << "Trace when file agent is disabled.";
		m_fileAgent->enable(true);
		EXPECT_TRUE(m_fileAgent->isEnabled());
		TRACE_CHANNEL(m_channelName) << "Trace when file agent is enabled.";
		m_fileAgent->enable(false);
		EXPECT_FALSE(m_fileAgent->isEnabled());
		TRACE_CHANNEL(m_channelName) << "Trace when file agent is disabled again.";
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(1, traceFileLines.size());
		EXPECT_TRUE(assertTraceLine(traceFileLines[0], "Trace when file agent is enabled."));
	}

	TEST_F(TraceFileContentTest, testAddTraceForOtherChannelsDoesNotWriteIntoFile)
	{
		TRACE_CHANNEL("OtherChannel") << "Trace for another channel";
		TRACE_CHANNEL(m_channelName) << "Trace for the channel with a file agent up";
		TRACE_CHANNEL("OtherChannel2") << "Trace for another channel";
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(1, traceFileLines.size());
		EXPECT_TRUE(assertTraceLine(traceFileLines[0], "Trace for the channel with a file agent up"));
	}

	TEST_F(TraceFileContentTest, testAddTraceAppendsToExistingFile)
	{
		writeTraceFile({ "Line1", "Line2", "Line3" });

		TRACE_CHANNEL_SEVERITY(m_channelName, "ERROR") << "First trace line added.";
		TRACE_CHANNEL_SEVERITY(m_channelName, "WARNING") << "Second trace line added.";
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(5, traceFileLines.size());
		EXPECT_EQ("Line1", traceFileLines[0]);
		EXPECT_EQ("Line2", traceFileLines[1]);
		EXPECT_EQ("Line3", traceFileLines[2]);
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[3], "ERROR", "First trace line added."));
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[4], "WARNING", "Second trace line added."));
	}

}}}

