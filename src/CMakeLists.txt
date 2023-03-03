
add_library(kex kex/texture.cpp)
if (KEX_USE_GLEW)
    target_link_libraries(kex glew_s)
endif()
