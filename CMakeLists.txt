include(ExternalProject)

cmake_minimum_required (VERSION 3.3)

set(CPPAST_BUILD_TEST OFF CACHE BOOL "")
set(CPPAST_BUILD_EXAMPLE OFF CACHE BOOL "")
set(CPPAST_BUILD_TOOL OFF CACHE BOOL "")

add_subdirectory(dependencies/cppast)
add_subdirectory(dependencies/cxxopts)

project (reflgen)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
else()
    set(CXX_FLAGS "-Wall -Wextra -Weffc++")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FLAGS}")

add_executable(reflgen
    src/cppast/extensions.h
    src/reflgen/generators.h
    src/reflgen/helpers.h
    src/linq.h
    src/main.cpp
    src/ranges.h
    src/utilities/string.h
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(reflgen PRIVATE /std:c++17)
else()
    target_compile_options(reflgen PRIVATE -std=c++1z)
endif()
target_include_directories(reflgen PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(reflgen PRIVATE cppast cxxopts)

