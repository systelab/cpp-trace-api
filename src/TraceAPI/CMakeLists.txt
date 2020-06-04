cmake_minimum_required(VERSION 3.2)

# Find external dependencies
set(BOOST_PACKAGE_NAME boost)
find_package(${BOOST_PACKAGE_NAME})
if (NOT TARGET ${BOOST_PACKAGE_NAME}::${BOOST_PACKAGE_NAME})
    set(BOOST_PACKAGE_NAME Boost)
    find_package(${BOOST_PACKAGE_NAME})
endif()

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure TraceAPI static library
set(TRACE_API_LIBRARY TraceAPI)
file(GLOB_RECURSE TRACE_API_LIBRARY_SRC "*.cpp")
file(GLOB_RECURSE TRACE_API_LIBRARY_HDR "*.h")
add_library(${TRACE_API_LIBRARY} STATIC ${TRACE_API_LIBRARY_SRC} ${TRACE_API_LIBRARY_HDR})
target_link_libraries(${TRACE_API_LIBRARY} ${BOOST_PACKAGE_NAME}::${BOOST_PACKAGE_NAME})

#Configure source groups
foreach(FILE ${TRACE_API_LIBRARY_SRC} ${TRACE_API_LIBRARY_HDR}) 
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

