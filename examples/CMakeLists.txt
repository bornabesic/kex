
add_executable(kex-demo demo.cpp)
target_compile_definitions(
    kex-demo
    PRIVATE
    PROJECT_NAME="${CMAKE_PROJECT_NAME}"
    PROJECT_VERSION="${CMAKE_PROJECT_VERSION}"
)
