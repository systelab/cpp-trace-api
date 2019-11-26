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
* Channel enabling/disabling
* Backup generation
* Deletion of old backups (rolling basis)

## Setup

### Build from sources

Prerequisites:
  - [Git](https://git-scm.com/)
  - [Conan](https://conan.io/)
  - [CMake](https://cmake.org/)
  - [Visual Studio](https://visualstudio.microsoft.com/) (only on Windows)
  - [GCC](https://gcc.gnu.org/) (only on Linux)

Build library with the following steps:
  1. Clone this repository in a local drive
  2. Make a build directory (i.e. `build/`)
  3. Register bintray repository for Systelab libraries on conan
  4. Install `conan` dependencies in the build directory
  5. Run `cmake` in the build directory to configure build targets
  6. Use `Visual Studio` (on Windows) or `make` (on Linux) to build the library

#### Windows

In order to build the application on Windows for the `Release` configuration, run the following commands ($VSINSTALLPATH is the path where Visual Studio has been installed):

``` bash
> git clone https://github.com/systelab/cpp-trace-api
> md build && cd build
> conan remote add systelab-bintray https://api.bintray.com/conan/systelab/conan
> conan install .. -s build_type=Release -s compiler.toolset=v141 -s arch=x86_64
> cmake .. -G "Visual Studio 15 2017 Win64"
> "$VSINSTALLPATH/devenv.com" TraceAPI.sln /build "Release" /PROJECT "TraceAPI"
```

However, if you want to `Debug` the source code, you will need these commands:

``` bash
> git clone https://github.com/systelab/cpp-trace-api
> md build && cd build
> conan remote add systelab-bintray https://api.bintray.com/conan/systelab/conan
> conan install .. -s build_type=Debug -s compiler.toolset=v141 -s arch=x86_64
> cmake .. -G "Visual Studio 15 2017 Win64"
> "$VSINSTALLPATH/devenv.com" TraceAPI.sln /build "Debug" /PROJECT "TraceAPI"
```

#### Linux
``` bash
> git clone https://github.com/systelab/cpp-trace-api
> mkdir build && cd build
> conan remote add systelab-bintray https://api.bintray.com/conan/systelab/conan
> conan install ..
> cmake .. -DCMAKE_BUILD_TYPE=[Debug | Coverage | Release]
> make
```

### Download using Conan

  1. Create/update your `conanfile.txt` to add this library as follows:

```
[requires]
TraceAPI/1.0.0@systelab/stable

[generators]
cmake
```

  2. Integrate Conan into CMake by adding the following code into your `CMakeLists.txt`:

```cmake
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
```

  3. Link against `${CONAN_LIBS}` when configuring your executables in CMake:

```cmake
set(MY_PROJECT MyProject)
add_executable(${MY_PROJECT} main.cpp)
target_link_libraries(${MY_PROJECT} ${CONAN_LIBS})
```

## Usage

### Set up a file agent

For each channel of traces to set up, a `systelab::trace::FileAgent` needs to be instantiated. 

The constructor parameters of this class allow defining the configuration of the associated traces file:
* Channel name
* Trace filename
* Trace folder path
* Maximum number of trace backups 

```
#include "TraceAPI/FileAgent.h"

std::string channelName = "MyChannel";
std::string traceFileName = "MyTraceFile";
std::string tracesFolderPath = "./Subfolder/MyTraces";
unsigned int nArchivedTraceFiles = 3;
auto fileAgent = std::make_unique<systelab::trace::FileAgent>(channelName, traceFileName, tracesFolderPath, nArchivedTraceFiles);
```

> The agent instance must be kept alive (not destroyed) during the whole application lifecycle. Thus, all traces added when there is no agent instance won't be recorded on the file.

### Add a trace

Traces can be added using the `TRACE_CHANNEL` macro providing the channel name as an argument and the trace content through the stream (`<<`) operator:

```cpp
#include "TraceAPI/ChannelMacro.h"

TRACE_CHANNEL("MyChannel") << "This is the trace number " << 1 << " to add";
```

It is highly recommended to define your own macros to easily trace to an specific channel:

```cpp
TODO
```

### Backup

In order to perform a backup of a traces file, just call the `backup()` method of the associated `FileAgent` entity.

```cpp
fileAgent->backup();
```

That would move the 

### Channel disabling

A channel can be temporarilly disabled by using the `enable()` method of the associated file agent:

```cpp
fileAgent->enable(false);
```

Similarly, the channel can be reenabled as follows:

```cpp
fileAgent->enable(true);
```
