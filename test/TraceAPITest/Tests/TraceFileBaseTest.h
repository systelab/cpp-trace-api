#include "stdafx.h"
#include "TraceAPI/ChannelMacro.h"
#include "TraceAPI/Configuration.h"
#include "TraceAPI/FileAgent.h"

#include <regex>


using namespace testing;

namespace systelab { namespace trace { namespace unit_test {

	class TraceFileBaseTest : public Test
	{
	public:
		void SetUp()
		{
			m_channelName = "MyTraceFile";
			m_baseFolderPath = "./Subfolder/MyTraces";
			m_rotationFoldersPrefix = "RotationPrefix";
			m_maxRotationDays = 3;

			m_traceFilepath = boost::filesystem::path(m_baseFolderPath) / (m_channelName + ".log");

			auto configuration = std::make_unique<Configuration>();
			configuration->setChannelName(m_channelName);
			configuration->setBaseFolderPath(m_baseFolderPath);
			configuration->setRotationFoldersPrefix(m_rotationFoldersPrefix);
			configuration->setMaxRotationDays(m_maxRotationDays);

			m_fileAgent = std::make_unique<FileAgent>(std::move(configuration));
			m_fileAgent->enable(true);
		}

		void TearDown()
		{
			m_fileAgent.reset();

			if (boost::filesystem::exists(m_traceFilepath))
			{
				boost::filesystem::remove(m_traceFilepath);
			}

			if (boost::filesystem::exists(boost::filesystem::path(m_baseFolderPath)))
			{
				boost::filesystem::remove_all(boost::filesystem::path(m_baseFolderPath));
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
										const std::string& expectedSeverity,
										const std::string& expectedMessage)
		{
			std::smatch match;
			std::regex re("([0-9: .-]+)[ ](.*)> (.*)", std::regex::extended);
			if (!std::regex_search(line, match, re) || match.size() != 3)
			{
				return AssertionFailure() << "The trace line does not satisfy the expected pattern";
			}

			std::string timestamp = match.str(1);
			// TODO: Validate timestamp

			std::string severity = match.str(2);
			std::string expectedSeverityBrackets = "[" + expectedSeverity + "]";
			if (severity != expectedSeverityBrackets)
			{
				return AssertionFailure() << "Different value for the trace line severity: " 
										  << "actual=" << severity << ", expected=" << expectedSeverityBrackets;
			}

			std::string message = match.str(3);
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
		std::string m_baseFolderPath;
		std::string m_rotationFoldersPrefix;
		unsigned int m_maxRotationDays;

		boost::filesystem::path m_traceFilepath;
	};

}}}
