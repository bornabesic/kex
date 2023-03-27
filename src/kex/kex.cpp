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

namespace kex {

    static constexpr float quad_data[] = {
            -1.f, -1.f,
            1.f, -1.f,
            1.f, 1.f,
            -1.f, 1.f,
    };

    static GLuint quad_buffer = 0;

    static constexpr auto vertex_shader_source = R"(
        #version 300 es

        void main() {
            gl_Position = vec4(0, 0, 0, 0);
        }
    )";

    static constexpr auto fragment_shader_source = R"(
        #version 300 es

        out highp vec4 color;

        void main() {
            color = vec4(1, 0, 0, 1);
        }
    )";

    void initialize() {
#ifdef KEX_USE_GLEW
        glewExperimental = GL_TRUE;
        const GLenum glew_status = glewInit();
        if (glew_status != GLEW_OK) {
            throw std::runtime_error(reinterpret_cast<const char *>(glewGetErrorString(glew_status)));
        }
#endif

        // Initialize the quad buffer
        glGenBuffers(1, &quad_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
        glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), quad_data, GL_STATIC_DRAW);

        // Initialize shaders
        VertexShader vertex_shader(vertex_shader_source);
        FragmentShader fragment_shader(fragment_shader_source);
        Program program(vertex_shader, fragment_shader);

        // Use the program
        program.use();

        std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';
    }

    void shutdown() {
        glDeleteBuffers(1, &quad_buffer);
    }

}
