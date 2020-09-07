#include "stdafx.h"
#include "TraceFileBaseTest.h"


using namespace testing;

namespace systelab { namespace trace { namespace unit_test {

	class TraceSeverityFilterTest : public TraceFileBaseTest
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

		void configureSeverityFilter(const std::vector<std::string>& severityFilter)
		{
			m_severityFilter = severityFilter;
			m_fileAgent = std::make_unique<FileAgent>(buildConfiguration());
			m_fileAgent->enable(true);
		}
	};


	TEST_F(TraceSeverityFilterTest, testOnlyErrorTracesAddedToFile)
	{
		configureSeverityFilter({ "ERROR" });

		for (unsigned int i = 1; i <= 3; i++)
		{
			TRACE_CHANNEL_SEVERITY(m_channelName, "INFO") << "This is INFO trace " << i << ".";
			TRACE_CHANNEL_SEVERITY(m_channelName, "DEBUG") << "This is DEBUG trace " << i << ".";
			TRACE_CHANNEL_SEVERITY(m_channelName, "ERROR") << "This is ERROR trace " << i << ".";
		}
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(3, traceFileLines.size());
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[0], "ERROR", "This is ERROR trace 1."));
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[1], "ERROR", "This is ERROR trace 2."));
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[2], "ERROR", "This is ERROR trace 3."));
	}

	TEST_F(TraceSeverityFilterTest, testInfoAndErrorTracesAddedToFile)
	{
		configureSeverityFilter({ "INFO", "ERROR" });

		for (unsigned int i = 1; i <= 2; i++)
		{
			TRACE_CHANNEL_SEVERITY(m_channelName, "INFO") << "This is INFO trace " << i << ".";
			TRACE_CHANNEL_SEVERITY(m_channelName, "DEBUG") << "This is DEBUG trace " << i << ".";
			TRACE_CHANNEL_SEVERITY(m_channelName, "ERROR") << "This is ERROR trace " << i << ".";
		}
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(4, traceFileLines.size());
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[0], "INFO", "This is INFO trace 1."));
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[1], "ERROR", "This is ERROR trace 1."));
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[2], "INFO", "This is INFO trace 2."));
		EXPECT_TRUE(assertTraceLineSeverity(traceFileLines[3], "ERROR", "This is ERROR trace 2."));
	}

	TEST_F(TraceSeverityFilterTest, testOnlyErrorTracesWithTagAddedToFile)
	{
		configureSeverityFilter({ "DEBUG" });

		for (unsigned int i = 1; i <= 3; i++)
		{
			TRACE_CHANNEL_SEVERITY_TAG(m_channelName, "DEBUG", "LOOP" + std::to_string(i)) << "This is DEBUG trace " << i << ".";
			TRACE_CHANNEL_SEVERITY_TAG(m_channelName, "INFO",  "LOOP" + std::to_string(i)) << "This is INFO trace " << i << ".";
			TRACE_CHANNEL_SEVERITY_TAG(m_channelName, "ERROR", "LOOP" + std::to_string(i)) << "This is ERROR trace " << i << ".";
		}
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(3, traceFileLines.size());
		EXPECT_TRUE(assertTraceLineSeverityTag(traceFileLines[0], "DEBUG", "LOOP1", "This is DEBUG trace 1."));
		EXPECT_TRUE(assertTraceLineSeverityTag(traceFileLines[1], "DEBUG", "LOOP2", "This is DEBUG trace 2."));
		EXPECT_TRUE(assertTraceLineSeverityTag(traceFileLines[2], "DEBUG", "LOOP3", "This is DEBUG trace 3."));
	}

	TEST_F(TraceSeverityFilterTest, testTracesWithoutSeverityNotAddedToFile)
	{
		configureSeverityFilter({ "DEBUG" });

		TRACE_CHANNEL(m_channelName) << "This is no severity nor tag trace " << 1 << ".";
		TRACE_CHANNEL(m_channelName) << "This is no severity nor tag trace " << 2 << ".";
		TRACE_CHANNEL_TAG(m_channelName, "TAG1") << "This is no severity with tag '" << "TAG1" << "' trace.";
		TRACE_CHANNEL_TAG(m_channelName, "TAG2") << "This is no severity with tag '" << "TAG2" << "' trace.";
		m_fileAgent->flush();

		auto traceFileLines = readTraceFile();
		ASSERT_EQ(0, traceFileLines.size());
	}

}}}

