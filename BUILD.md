# Build from sources

## Prerequisites:
  - [Git](https://git-scm.com/)
  - [Conan](https://conan.io/)
  - [CMake](https://cmake.org/)
  - [Visual Studio](https://visualstudio.microsoft.com/) (only on Windows)
  - [GCC](https://gcc.gnu.org/) (only on Linux)

## Build steps

Build library with the following steps:
  1. Clone this repository in a local drive
  2. Make a build directory (i.e. `build/`)
  3. Install `conan` dependencies in the build directory
  4. Run `cmake` in the build directory to configure build targets
  5. Use `Visual Studio` (on Windows) or `make` (on Linux) to build the library

### Windows

In order to build the application on Windows for the `Release` configuration, run the following commands ($VSINSTALLPATH is the path where Visual Studio has been installed):

``` bash
> git clone https://github.com/systelab/cpp-trace-api
> md build && cd build
> conan remote add systelab-public https://systelab.jfrog.io/artifactory/api/conan/cpp-conan-production-local
> conan install .. -s build_type=Release -s compiler.toolset=v143 -s arch=x86_64
> cmake .. -G "Visual Studio 17 2022" -A x64
> "$VSINSTALLPATH/devenv.com" TraceAPI.sln /build "Release" /PROJECT "TraceAPI"
```

However, if you want to `Debug` the source code, you will need these commands:

``` bash
> git clone https://github.com/systelab/cpp-trace-api
> md build && cd build
> conan remote add systelab-public https://systelab.jfrog.io/artifactory/api/conan/cpp-conan-production-local
> conan install .. -s build_type=Debug -s compiler.toolset=v143 -s arch=x86_64
> cmake .. -G "Visual Studio 17 2022" -A x64
> "$VSINSTALLPATH/devenv.com" TraceAPI.sln /build "Debug" /PROJECT "TraceAPI"
```

### Linux

``` bash
> git clone https://github.com/systelab/cpp-trace-api
> mkdir build && cd build
> conan remote add systelab-public https://systelab.jfrog.io/artifactory/api/conan/cpp-conan-production-local
> conan install ..
> cmake .. -DCMAKE_BUILD_TYPE=[Debug | Coverage | Release]
> make
```
