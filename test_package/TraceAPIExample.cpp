#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>

#include "TraceAPI/TraceAgent.h"


int main(int argc, char *argv[])
{
	auto agent = std::make_unique<systelab::trace::TraceAgent>("Channel", "Filename", "FolderPath", 3);
	agent->enable(true);

    return 0;
}
