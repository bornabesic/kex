
FetchContent_Declare(
    SDL2
    URL https://github.com/libsdl-org/SDL/releases/download/release-2.26.3/SDL2-2.26.3.tar.gz
    URL_HASH MD5=6ff7cbe9f77fc5afa79d54895dc6becc
)
FetchContent_MakeAvailable(SDL2)
include_directories(${SDL2_SOURCE_DIR}/include)

add_executable(kex-sdl2-demo src/main.cpp)
target_compile_definitions(
    kex-sdl2-demo
    PRIVATE
    PROJECT_NAME="${CMAKE_PROJECT_NAME}"
    PROJECT_VERSION="${CMAKE_PROJECT_VERSION}"
)
target_link_libraries(kex-sdl2-demo kex SDL2)
