#include "stdafx.h"
#include "TraceAPI/ChannelMacro.h"
#include "TraceAPI/FileAgent.h"


using namespace testing;

namespace systelab { namespace trace { namespace unit_test {

	class TraceFileContentTest : public Test
	{
	public:
		void SetUp()
		{
			m_channelName = "MyChannel";
			m_traceFileName = "MyTraceFile";
			m_tracesFolderPath = "./Subfolder/MyTraces";
			m_nArchivedTraceFiles = 3;

			m_traceFilepath = boost::filesystem::path(m_tracesFolderPath) / (m_traceFileName + ".log");

			m_fileAgent = std::make_unique<FileAgent>(m_channelName, m_traceFileName, m_tracesFolderPath, m_nArchivedTraceFiles);
		}

		void TearDown()
		{
			m_fileAgent.reset();

			if (boost::filesystem::exists(m_traceFilepath))
			{
				boost::filesystem::remove(m_traceFilepath);
			}
		}

	protected:
		std::unique_ptr<FileAgent> m_fileAgent;

		std::string m_channelName;
		std::string m_traceFileName;
		std::string m_tracesFolderPath;
		unsigned int m_nArchivedTraceFiles;

		boost::filesystem::path m_traceFilepath;
	};


	TEST_F(TraceFileContentTest, testAddTraceCreatesFile)
	{
		m_fileAgent->enable(true);

		TRACE_CHANNEL(m_channelName) << "Log here" << 123 << "Patata";
		TRACE_CHANNEL(m_channelName) << "Log here" << 456 << "Patata";
		TRACE_CHANNEL(m_channelName) << "Log here" << 789 << "Patata";

		m_fileAgent->flush();

	}

}}}

