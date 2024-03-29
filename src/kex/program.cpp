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

#include <kex/program.hpp>

#include <glad/gles2.h>

namespace kex {

    class Program::Impl {
    public:
        Impl(const kex::VertexShader &vertex_shader, const kex::FragmentShader &fragment_shader) {
            id = glCreateProgram();
            glAttachShader(id, vertex_shader.id());
            glAttachShader(id, fragment_shader.id());
            glLinkProgram(id);
        }

        int get_uniform_location(const char *name) const {
            return glGetUniformLocation(id, name);
        }

        void use() const {
            glUseProgram(id);
        }

        ~Impl() {
            glDeleteProgram(id);
        }

    private:
        GLuint id = 0;
    };

    Program::Program(const kex::VertexShader &vertex_shader, const kex::FragmentShader &fragment_shader) : impl(
            std::make_unique<Impl>(vertex_shader, fragment_shader)) {}

    void Program::use() const { impl->use(); }

    int Program::get_uniform_location(const char *name) const { return impl->get_uniform_location(name); }

    Program::~Program() = default;

}
