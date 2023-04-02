
add_library(glad STATIC glad/gles2.c)
set_property(TARGET glad PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(
        kex
        SHARED
        kex/kex.cpp
        kex/texture.cpp
        kex/sprite.cpp
        kex/spritebatch.cpp
        kex/font.cpp
        kex/text.cpp
        kex/shader.cpp
        kex/program.cpp
        kex/buffer.cpp
        kex/vertexarray.cpp
        kex/utils.cpp
)
target_link_libraries(kex glad)
