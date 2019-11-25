#include "stdafx.h"
#include "TraceAPI/ChannelMacro.h"
#include "TraceAPI/FileAgent.h"

#include <regex>


using namespace testing;

namespace systelab { namespace trace { namespace unit_test {

	class TraceFileBaseTest : public Test
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
			m_fileAgent->enable(true);
		}

		void TearDown()
		{
			m_fileAgent.reset();

			if (boost::filesystem::exists(m_traceFilepath))
			{
				boost::filesystem::remove(m_traceFilepath);
			}
		}

		std::vector<std::string> readTraceFile()
		{
			std::vector<std::string> lines;

			std::ifstream ifs(m_traceFilepath.string());
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

		void writeTraceFile(std::vector<std::string> lines)
		{
			boost::filesystem::remove(m_traceFilepath);

			std::string fileContents = "";
			for (auto line : lines)
			{
				fileContents += line;
				fileContents += "\n";
			}

			std::ofstream fileStream;
			fileStream.open(m_traceFilepath.string());
			fileStream << fileContents;
			fileStream.close();
		}

		AssertionResult assertTraceLine(const std::string& line,
										const std::string& expectedChannel,
										const std::string& expectedMessage)
		{
			std::smatch match;
			std::regex re("[0-9-: .]+(.*)> (.*)", std::regex::extended);
			if (!std::regex_search(line, match, re) || match.size() != 3)
			{
				return AssertionFailure() << "The trace line does not satisfy the expected pattern";
			}

			std::string channel = match.str(1);
			std::string expectedChannelBrackets = "[" + expectedChannel + "]";
			if (channel != expectedChannelBrackets)
			{
				return AssertionFailure() << "Different value for the trace line channel: " 
										  << "actual=" << channel << ", expected=" << expectedChannelBrackets;
			}

			std::string message = match.str(2);
			if (message != expectedMessage)
			{
				return AssertionFailure() << "Different value for the trace line message: "
										  << "actual=" << message << ", expected=" << expectedMessage;
			}

			return AssertionSuccess();
		}

	protected:
		std::unique_ptr<FileAgent> m_fileAgent;

		std::string m_channelName;
		std::string m_traceFileName;
		std::string m_tracesFolderPath;
		unsigned int m_nArchivedTraceFiles;

		boost::filesystem::path m_traceFilepath;
	};

}}}
