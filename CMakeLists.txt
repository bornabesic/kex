cmake_minimum_required(VERSION 3.00)
project(
    kex
    VERSION 0.1.0
    LANGUAGES CXX
    HOMEPAGE_URL https://github.com/bornabesic/kex
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("${CMAKE_PROJECT_NAME} version: ${CMAKE_PROJECT_VERSION}")

# Options
option(KEX_USE_GLEW "Use GLEW" ON)
option(KEX_BUILD_EXAMPLES "Build examples" OFF)

add_definitions(-DKEX_USE_GLEW=$<BOOL:KEX_USE_GLEW>)

# Dependencies
include(FetchContent)

# GLEW
if (KEX_USE_GLEW)
    add_definitions(-DGLEW_NO_GLU)
    set(OpenGL_GL_PREFERENCE GLVND)
    FetchContent_Declare(
            glew
            URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz
            URL_HASH MD5=3579164bccaef09e36c0af7f4fd5c7c7
    )
    FetchContent_MakeAvailable(glew)
    add_subdirectory(${glew_SOURCE_DIR}/build/cmake ${glew_BINARY_DIR})
    include_directories(${glew_SOURCE_DIR}/include)
endif ()

include_directories(include)
add_subdirectory(src)

# Examples
if (KEX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
