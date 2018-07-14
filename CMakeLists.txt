include(ExternalProject)

cmake_minimum_required (VERSION 3.3)

set(CPPAST_BUILD_TEST OFF)
set(CPPAST_BUILD_EXAMPLE OFF)
set(CPPAST_BUILD_TOOL OFF)

add_subdirectory(dependencies/cppast)

project (reflgen)

if (not MSVC)
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
target_include_directories(reflgen PRIVATE ${CMAKE_SOURCE_DIR}/src)
if (MSVC)
    target_compile_options(reflgen PRIVATE /std:c++17)
else()
    target_compile_options(reflgen PRIVATE -std=c++17)
endif()
target_link_libraries(reflgen PRIVATE cppast cxxopts)

