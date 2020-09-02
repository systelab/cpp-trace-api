#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>

#include "TraceAPI/ChannelMacro.h"
#include "TraceAPI/Configuration.h"
#include "TraceAPI/FileAgent.h"


int main(int argc, char *argv[])
{
	auto configuration = std::make_unique<systelab::trace::Configuration>();
	configuration->setChannelName("MyChannel");
	configuration->setBaseFolderPath("FolderPath");

	auto fileAgent = std::make_unique<systelab::trace::FileAgent>(std::move(configuration));
	fileAgent->enable(true);
	TRACE_CHANNEL("Channel") << "The Conan package has been created successfully!";
	fileAgent->flush();

    return 0;
}
