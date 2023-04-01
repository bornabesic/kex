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

#include <kex/shader.hpp>

#include <iostream>
#include <cstdlib>

#include <glad/gles2.h>

void panic_on_error(GLuint shader) {
    GLint is_shader_compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_shader_compiled);
    if (is_shader_compiled != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shader, 1024, &log_length, message);
        std::cout << std::string(message, log_length) << '\n';
        std::exit(1);
    }
}

namespace kex {

    template<ShaderType T>
    class Shader<T>::Impl {
    public:
        explicit Impl(const std::string &source) {
            GLenum type_gl;
            if constexpr (T == ShaderType::VERTEX) {
                type_gl = GL_VERTEX_SHADER;
            } else if constexpr (T == ShaderType::FRAGMENT) {
                type_gl = GL_FRAGMENT_SHADER;
            }

            const auto *source_ptr = source.c_str();
            id = glCreateShader(type_gl);
            glShaderSource(id, 1, &source_ptr, nullptr);
            glCompileShader(id);
            panic_on_error(id);
        }

        ~Impl() {
            glDeleteShader(id);
        }

    private:
        GLuint id = 0;

        friend Shader<T>;
    };

    template<ShaderType T>
    Shader<T>::Shader(const std::string &source) : impl(std::make_unique<Impl>(source)) {}

    template<ShaderType T>
    constexpr ShaderType Shader<T>::type() const { return T; }

    template<ShaderType T>
    unsigned int Shader<T>::id() const { return impl->id; }

    template<ShaderType T>
    Shader<T>::~Shader() = default;

    // Specializations
    template
    class Shader<ShaderType::VERTEX>;

    template
    class Shader<ShaderType::FRAGMENT>;

}
