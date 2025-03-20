cmake_minimum_required(VERSION 3.2)

project(TraceAPIPackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find tested package
find_package(TraceAPI)

# Configure test package project
set(TRACE_API_PACKAGE_TEST_PROJECT TraceAPIPackageTest)
add_executable(${TRACE_API_PACKAGE_TEST_PROJECT} TraceAPIExample.cpp)
target_link_libraries(${TRACE_API_PACKAGE_TEST_PROJECT} TraceAPI::TraceAPI)

# Register test
enable_testing()
add_test(NAME TraceAPIPackageTest COMMAND ${TRACE_API_PACKAGE_TEST_PROJECT})
