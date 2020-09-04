#include "stdafx.h"
#include "TraceFileBaseTest.h"


using namespace testing;

namespace systelab { namespace trace { namespace unit_test {

	class TraceFileFieldsTest : public TraceFileBaseTest
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


	TEST_F(TraceFileFieldsTest, testAddSeverityTraces)
	{
		TRACE_CHANNEL_SEVERITY(m_channelName, "INFO")  << "This trace entry has " << "INFO"  << " severity.";
		TRACE_CHANNEL_SEVERITY(m_channelName, "DEBUG") << "This trace entry has " << "DEBUG" << " severity.";
		TRACE_CHANNEL_SEVERITY(m_channelName, "ERROR") << "This trace entry has " << "ERROR" << " severity.";
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(3, traceFileLines.size());
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[0], "INFO",  "This trace entry has INFO severity."));
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[1], "DEBUG", "This trace entry has DEBUG severity."));
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[2], "ERROR", "This trace entry has ERROR severity."));
	}

	TEST_F(TraceFileFieldsTest, testAddTagTraces)
	{
		TRACE_CHANNEL_TAG(m_channelName, "TAG1") << "This trace entry is associated with " << "TAG1" << " tag.";
		TRACE_CHANNEL_TAG(m_channelName, "TAG2") << "This trace entry is associated with " << "TAG2" << " tag.";
		TRACE_CHANNEL_TAG(m_channelName, "TAG3") << "This trace entry is associated with " << "TAG3" << " tag.";
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(3, traceFileLines.size());
		EXPECT_TRUE(assertTraceLineTag(traceFileLines[0], "TAG1", "This trace entry is associated with TAG1 tag."));
		EXPECT_TRUE(assertTraceLineTag(traceFileLines[1], "TAG2", "This trace entry is associated with TAG2 tag."));
		EXPECT_TRUE(assertTraceLineTag(traceFileLines[2], "TAG3", "This trace entry is associated with TAG3 tag."));
	}

	TEST_F(TraceFileFieldsTest, testAddSeverityTagTraces)
	{
		TRACE_CHANNEL_SEVERITY_TAG(m_channelName, "INFO",  "TAG1") << "This is line " << 1 << " of severity-tag traces.";
		TRACE_CHANNEL_SEVERITY_TAG(m_channelName, "DEBUG", "TAG2") << "This is line " << 2 << " of severity-tag traces.";
		TRACE_CHANNEL_SEVERITY_TAG(m_channelName, "ERROR", "TAG3") << "This is line " << 3 << " of severity-tag traces.";
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(3, traceFileLines.size());
		EXPECT_TRUE(assertTraceLineSeverityTag(traceFileLines[0], "INFO",  "TAG1", "This is line 1 of severity-tag traces."));
		EXPECT_TRUE(assertTraceLineSeverityTag(traceFileLines[1], "DEBUG", "TAG2", "This is line 2 of severity-tag traces."));
		EXPECT_TRUE(assertTraceLineSeverityTag(traceFileLines[2], "ERROR", "TAG3", "This is line 3 of severity-tag traces."));
	}

}}}

