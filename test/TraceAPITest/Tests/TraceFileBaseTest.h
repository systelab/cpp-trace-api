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
										const std::string& expectedMessage)
		{
			std::smatch match;
			std::regex re("(.*)> (.*)", std::regex::ECMAScript);
			if (!std::regex_search(line, match, re) || match.size() != 3)
			{
				return AssertionFailure() << "The trace line does not satisfy the expected pattern";
			}

			std::string timestamp = match.str(1);
			if (!isTimestampFormatValid(timestamp))
			{
				return AssertionFailure() << "The trace line timestamp does not satisfy the expected pattern: "
					<< "actual=" << timestamp << ", expectedPattern=YYYY-MM-DD HH:MM:SS.dddddd (UTC+/-XX:YY)";
			}

			std::string message = match.str(2);
			if (message != expectedMessage)
			{
				return AssertionFailure() << "Different value for the trace line message: "
					<< "actual=" << message << ", expected=" << expectedMessage;
			}

			return AssertionSuccess();
		}

		AssertionResult assertTraceLineSeverity(const std::string& line,
												const std::string& expectedSeverity,
												const std::string& expectedMessage)
		{
			return assertTraceLineWithFields(line, { {"severity", expectedSeverity} }, expectedMessage);
		}

		AssertionResult assertTraceLineTag(const std::string& line,
										   const std::string& expectedTag,
										   const std::string& expectedMessage)
		{
			return assertTraceLineWithFields(line, { {"tag", expectedTag} }, expectedMessage);
		}

		AssertionResult assertTraceLineSeverityTag(const std::string& line,
												   const std::string& expectedSeverity,
												   const std::string& expectedTag,
												   const std::string& expectedMessage)
		{
			return assertTraceLineWithFields(line, { {"severity", expectedSeverity}, {"tag", expectedTag} }, expectedMessage);
		}

		AssertionResult assertTraceLineWithFields(const std::string& line,
												  const std::map<std::string, std::string>& expectedFields,
												  const std::string& expectedMessage)
		{
			std::string regexStr = "([^\\[]*)"; // Timestamp group
			regexStr += (expectedFields.size() > 0) ? " " : ""; // Fields separator
			for (auto expectedField : expectedFields)
			{
				regexStr += "(\\[[^\\]]*\\])"; // Field group
			}
			regexStr += "> (.*)"; // Message group

			std::smatch match;
			unsigned int expectedMatchSize = 3 + (unsigned int) expectedFields.size();
			std::regex re(regexStr, std::regex::ECMAScript);
			if (!std::regex_search(line, match, re) || match.size() != expectedMatchSize)
			{
				return AssertionFailure() << "The trace line does not satisfy the expected pattern";
			}

			std::string timestamp = match.str(1);
			if (!isTimestampFormatValid(timestamp))
			{
				return AssertionFailure() << "The trace line timestamp does not satisfy the expected pattern: "
										  << "actual=" << timestamp << ", expectedPattern=YYYY-MM-DD HH:MM:SS.dddddd (UTC+/-XX:YY)";
			}

			unsigned int fieldIndex = 0;
			for (auto expectedField : expectedFields)
			{
				std::string fieldValue = match.str(2 + fieldIndex);
				std::string expectedFieldValueBrackets = "[" + expectedField.second + "]";
				if (fieldValue != expectedFieldValueBrackets)
				{
					return AssertionFailure() << "Different value for the trace line " + expectedField.first + ": "
											  << "actual=" << fieldValue << ", expected=" << expectedFieldValueBrackets;
				}

				fieldIndex++;
			}

			std::string message = match.str(expectedMatchSize - 1);
			if (message != expectedMessage)
			{
				return AssertionFailure() << "Different value for the trace line message: "
										  << "actual=" << message << ", expected=" << expectedMessage;
			}

			return AssertionSuccess();
		}

		bool isTimestampFormatValid(const std::string& timestamp)
		{
			std::smatch match;
			std::regex re("^[0-9]{4}-[0-1][0-9]-[0-3][0-9] [0-2][0-9]:[0-5][0-9]:[0-5][0-9].[0-9]{6} \\(UTC[+-][0-1][0-9]:[0-5][0-9]\\)$", std::regex::ECMAScript);
			return std::regex_search(timestamp, match, re);

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
