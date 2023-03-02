cmake_minimum_required(VERSION 3.00)
project(
    kex
    VERSION 0.1.0
    LANGUAGES CXX
    HOMEPAGE_URL https://github.com/bornabesic/kex
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("${CMAKE_PROJECT_NAME} version: ${CMAKE_PROJECT_VERSION}")

# Options
option(KEX_BUILD_EXAMPLES OFF)

if (KEX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
