/*
Kex: Plug-and-play 2D graphics C++ library built on top of OpenGL ES 3.0 API
Copyright (C) 2023  Borna Bešić

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdexcept>
#include <iostream>

#ifdef KEX_USE_GLEW
#include "GL/glew.h"
#endif

#include <kex/kex.h>
#include <kex/shader.h>
#include <kex/program.h>
#include <kex/sprite.h>


namespace kex {

    static constexpr float quad_data[] = {
            -.5f, -.5f,
            .5f, -.5f,
            -.5f, .5f,
            .5f, .5f,
    };

    SpriteBuffers sprite_buffers;
    unsigned int sprite_vao = 0;

    static constexpr auto vertex_shader_source = R"(
        #version 300 es

        layout (location = 0) in highp vec2 base_position_in;
        layout (location = 1) in highp vec2 position_in;
        layout (location = 2) in highp vec2 tex_coords_in;

        out highp vec2 tex_coords;

        void main() {
            gl_Position = vec4(base_position_in, 0, 1);
            tex_coords = tex_coords_in;
        }
    )";

    static constexpr auto fragment_shader_source = R"(
        #version 300 es

        uniform sampler2D tex_u;

        in highp vec2 tex_coords;

        out highp vec4 color_out;

        void main() {
            color_out = texture(tex_u, tex_coords);
        }
    )";

    std::unique_ptr<VertexShader> vertex_shader;
    std::unique_ptr<FragmentShader> fragment_shader;
    std::unique_ptr<Program> program;

    void initialize() {
#ifdef KEX_USE_GLEW
        glewExperimental = GL_TRUE;
        const GLenum glew_status = glewInit();
        if (glew_status != GLEW_OK) {
            throw std::runtime_error(reinterpret_cast<const char *>(glewGetErrorString(glew_status)));
        }
#endif

        // Generate VAO for sprites
        glGenVertexArrays(1, &sprite_vao);
        glBindVertexArray(sprite_vao);

        // Initialize the quad buffer
        glGenBuffers(1, &sprite_buffers.base_positions);
        glBindBuffer(GL_ARRAY_BUFFER, sprite_buffers.base_positions);
        glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), quad_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glVertexAttribDivisor(0, 0); // Same for all sprites

        // Initialize the positions buffer
        glGenBuffers(1, &sprite_buffers.positions);
        glBindBuffer(GL_ARRAY_BUFFER, sprite_buffers.positions);
        glBufferData(GL_ARRAY_BUFFER, max_sprite_instances * 4 * 2 * sizeof(float), nullptr, GL_STREAM_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glVertexAttribDivisor(1, 1); // One per sprite

        // Initialize the texture coordinates buffer
        glGenBuffers(1, &sprite_buffers.tex_coords);
        glBindBuffer(GL_ARRAY_BUFFER, sprite_buffers.tex_coords);
        glBufferData(GL_ARRAY_BUFFER, max_sprite_instances * 4 * 2 * sizeof(float), nullptr, GL_STREAM_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        // TODO glVertexAttribDivisor(2, 1); // One per sprite

        // Initialize shaders
        vertex_shader = std::make_unique<VertexShader>(vertex_shader_source);
        fragment_shader = std::make_unique<FragmentShader>(fragment_shader_source);
        program = std::make_unique<Program>(*vertex_shader, *fragment_shader);

        program->use();

        GLint texture_location = program->get_uniform_location("tex_u");
        glUniform1i(texture_location, 0);

        std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';
    }

    void shutdown() {
        glDeleteBuffers(1, &sprite_buffers.base_positions);
        glDeleteBuffers(1, &sprite_buffers.positions);
        glDeleteBuffers(1, &sprite_buffers.tex_coords);
    }

}
