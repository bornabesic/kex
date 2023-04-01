cmake_minimum_required(VERSION 3.00)
project(
    kex
    VERSION 0.1.0
    LANGUAGES CXX C
    HOMEPAGE_URL https://github.com/bornabesic/kex
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("${CMAKE_PROJECT_NAME} version: ${CMAKE_PROJECT_VERSION}")

# Options
option(KEX_BUILD_EXAMPLES "Build examples" OFF)

message("KEX_BUILD_EXAMPLES: ${KEX_BUILD_EXAMPLES}")

# Dependencies
include(FetchContent)

# stb
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb
)
FetchContent_MakeAvailable(stb)
include_directories(${stb_SOURCE_DIR})

# Kex
include_directories(include)
add_subdirectory(src)

# Examples
if (KEX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
