
add_library(kex STATIC kex/kex.cpp kex/texture.cpp kex/sprite.cpp kex/spritebatch.cpp)
if (KEX_USE_GLEW)
    target_link_libraries(kex glew_s)
endif()
