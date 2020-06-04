cmake_minimum_required(VERSION 3.2)

# Enable testing module
enable_testing()

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find external dependencies
set(GTEST_PACKAGE_NAME gtest)
find_package(${GTEST_PACKAGE_NAME})
if (NOT TARGET ${GTEST_PACKAGE_NAME}::${GTEST_PACKAGE_NAME})
    set(GTEST_PACKAGE_NAME GTest)
    find_package(${GTEST_PACKAGE_NAME})
endif()

# Configure test project
set(TRACE_API_TEST_PROJECT TraceAPITest)
file(GLOB_RECURSE TRACE_API_TEST_PROJECT_SRC "*.cpp")
file(GLOB_RECURSE TRACE_API_TEST_PROJECT_HDR "*.h")
add_executable(${TRACE_API_TEST_PROJECT} ${TRACE_API_TEST_PROJECT_SRC} ${TRACE_API_TEST_PROJECT_HDR})
target_link_libraries(${TRACE_API_TEST_PROJECT} TraceAPI ${GTEST_PACKAGE_NAME}::${GTEST_PACKAGE_NAME})

# Ignored missing PDBs link warning on Visual Studio
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set_target_properties(${TRACE_API_TEST_PROJECT} PROPERTIES LINK_FLAGS "/ignore:4099")
endif()

#Configure source groups
foreach(FILE ${TRACE_API_TEST_PROJECT_SRC} ${TRACE_API_TEST_PROJECT_HDR}) 
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

# Register tests
add_test(NAME TraceAPITest COMMAND ${TRACE_API_TEST_PROJECT})

# Additional target for code coverage analysis
if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	include(CodeCoverage)
	setup_target_for_coverage(TraceAPITestCoverage TraceAPITest coverage)
endif()
