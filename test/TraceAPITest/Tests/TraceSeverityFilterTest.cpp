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

}}}

