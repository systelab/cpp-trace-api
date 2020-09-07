[![Build Status](https://travis-ci.org/systelab/cpp-trace-api.svg?branch=master)](https://travis-ci.org/systelab/cpp-trace-api)
[![Build status](https://ci.appveyor.com/api/projects/status/h5mevc6dpunulq13?svg=true)](https://ci.appveyor.com/project/systelab/cpp-trace-api)
[![codecov](https://codecov.io/gh/systelab/cpp-trace-api/branch/master/graph/badge.svg)](https://codecov.io/gh/systelab/cpp-trace-api)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/c39bf050b7524110ae9915238c0337d0)](https://www.codacy.com/manual/systelab/cpp-trace-api?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=systelab/cpp-trace-api&amp;utm_campaign=Badge_Grade)
[![Download](https://api.bintray.com/packages/systelab/conan/TraceAPI:systelab/images/download.svg)](https://bintray.com/systelab/conan/TraceAPI:systelab/_latestVersion)


# C++ Trace System

This library provides utilities to easily add traces to your application.

## Supported features

* Multiple channels
* Dump to .log files
* Dedicated threads
* Built-in fields
* Channel enabling/disabling
* Severity filtering
* Backup generation
* Deletion of old backups (rolling basis)


## Setup

### Download using Conan

This library is designed to be installed by making use of [Conan](https://conan.io/) package manager. So, you just need to add the following requirement into your Conan recipe:

```python
def requirements(self):
   self.requires("TraceAPI/1.0.0@systelab/stable")
```

> Version number of this code snipped is set just as an example. Replace it for the desired package to retrieve.

As this package is not available on the conan-center, you will also need to configure a remote repository before installing dependencies:

```bash
conan remote add systelab-bintray https://api.bintray.com/conan/systelab/conan 
```

See Conan [documentation](https://docs.conan.io/en/latest/) for further details on how to integrate this package with your build system.

### Build from sources

See [BUILD.md](BUILD.md) document for details.


## Usage

### Set up a file agent

For each channel of traces to set up, a `systelab::trace::FileAgent` needs to be instantiated. 

The constructor parameter of this class requires a configuration object where the trace channel particularities are defined:
* Channel name
* Path of logs base folder
* Rotation folders prefix
* Maximum number of rotation folders
* ...

```cpp
#include "TraceAPI/Configuration.h"
#include "TraceAPI/FileAgent.h"

auto configuration = std::make_unique<systelab::trace::Configuration>();
configuration->setChannelName("MyChannel");
configuration->setBaseFolderPath("./Subfolder/MyTraces");
configuration->setMaxRotationDays(3);
			
auto fileAgent = std::make_unique<systelab::trace::FileAgent>(std::move(configuration));
```

> The agent instance must be kept alive (not destroyed) during the whole application lifecycle. Thus, all traces added when there is no agent instance won't be recorded on the file.

### Add a trace

Traces can be added using the `TRACE_CHANNEL` macro providing:
* Channel name as an argument
* Trace content through the stream (`<<`) operator

```cpp
#include "TraceAPI/ChannelMacro.h"

TRACE_CHANNEL("MyChannel") << "This is the trace number " << 1 << " to add";
```

It is highly recommended to define your own macros to easily trace to an specific channel:

```cpp
#define TRACE_MY_CHANNEL() \
    TRACE_CHANNEL("MyChannel")
```

Then, traces can be added as follows:

```cpp
TRACE_MY_CHANNEL() << "Trace added using custom macro.";
```

### Built-in fields

The library is also prepared to record the severity and a tag value for each trace added.

As the usage of these fields is optional, the library provides specific macros designed for that purpose:

```cpp
#include "TraceAPI/ChannelMacro.h"

TRACE_CHANNEL_SEVERITY("MyChannel", "INFO") << "This is a trace with 'INFO' severity";
TRACE_CHANNEL_TAG("MyChannel", "MY_TAG") << "This is a trace with 'MY_TAG' tag";
TRACE_CHANNEL_SEVERITY_TAG("MyChannel", "ERROR", "TAG2") << "An error trace with 'TAG2' tag";
```

### Rotation

Trace files are automatically rotated at midnight. However, if you want to force a log file rotation, just call the `rotate()` method of the associated `FileAgent` entity.

```cpp
fileAgent->rotate();
```

That would move the current traces file into a `Logs_YYYY_MM_DD` subfolder, where `YYYY`, `MM` and `DD` respectively correspond with the year, month and day of the current date. In order to allow archiving of multiple trace files for the same day, a timestamp is appended to trace the filename.

Additionally, old backups are automatically deleted, so only the configured amount of rotation days folders is kept.

Prefix of rotation folders can be customized by means of the `setRotationFoldersPrefix` of the configuration object (by default, it is set to `Logs`):

```cpp
configuration->setRotationFoldersPrefix("MyPrefix");
```

### Channel disabling

A channel can be temporarilly disabled by using the `enable()` method of the associated file agent:

```cpp
fileAgent->enable(false);
```

Similarly, the channel can be reenabled as follows:

```cpp
fileAgent->enable(true);
```

### Severity filtering

This library allows defining which severity levels will be logged on the trace files. These severities are set using the `setSeverityFilter()` method of the configuration object:

```cpp
std::vector<std::string> severityFilter = {"ERROR", "WARNING"};
configuration->setSeverityFilter(severityFilter);
```

> By default (when no filter is configured), traces for any severity level are added to files.


