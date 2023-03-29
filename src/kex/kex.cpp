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
#include <kex/buffer.h>
#include <kex/vertexarray.h>

namespace kex {

    std::unique_ptr<VertexArray> vao;

    static constexpr auto vertex_shader_source = R"(
        #version 300 es

        layout (location = 0) in highp vec2 base_position_in;
        layout (location = 1) in highp vec2 tex_coords_in;
        layout (location = 2) in highp vec2 position_in;
        layout (location = 3) in highp vec2 size_in;

        out highp vec2 tex_coords;

        void main() {
            highp vec2 position = (base_position_in * size_in + position_in) / vec2(800, 600);
            gl_Position = vec4(position, 0, 1);
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

//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    }

}
