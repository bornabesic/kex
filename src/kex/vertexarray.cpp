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

#include <kex/vertexarray.h>

#ifdef KEX_USE_GLEW
#include <GL/glew.h>
#endif

namespace kex {

    class VertexArray::Impl {
    public:
        Impl() {
            glGenVertexArrays(1, &id);
        }

        void bind() const {
            glBindVertexArray(id);
        }

        template<VertexAttr Attr, BufferUsage Usg, int Div, bool Norm>
        void add_attribute(const ArrayBuffer<Usg> &array_buffer) {
            this->bind();
            array_buffer.bind();

            GLint size;
            GLenum type;
            GLboolean normalized;
            if constexpr (Attr == VertexAttr::VEC2) {
                size = 2;
                type = GL_FLOAT;
            } else if constexpr (Attr == VertexAttr::VEC4) {
                size = 4;
                type = GL_FLOAT;
            }

            if constexpr (Norm) {
                normalized = GL_TRUE;
            } else {
                normalized = GL_FALSE;
            }

            glEnableVertexAttribArray(current_index);
            glVertexAttribPointer(current_index, size, type, normalized, 0, nullptr);
            glVertexAttribDivisor(current_index, Div);
            ++current_index;
        }

        ~Impl() {
            glDeleteVertexArrays(1, &id);
        }

    private:
        GLuint id = 0;
        GLuint current_index = 0;

        friend VertexArray;
    };

    VertexArray::VertexArray() : impl(std::make_unique<Impl>()) {}

    void VertexArray::bind() const { impl->bind(); }

    unsigned int VertexArray::get_id() const { return impl->id; }

    VertexArray::VertexArray(VertexArray &&vertex_array) noexcept = default;

    template<VertexAttr Attr, int Div, bool Norm, BufferUsage Usg>
    void
    VertexArray::add_attribute(const ArrayBuffer<Usg> &array_buffer) {
        impl->add_attribute<Attr, Usg, Div, Norm>(array_buffer);
    }

    VertexArray::~VertexArray() = default;

    // Specializations
    template void VertexArray::add_attribute<VertexAttr::VEC2, 0, false, BufferUsage::STATIC>(
            const ArrayBuffer<BufferUsage::STATIC> &array_buffer);

    template void VertexArray::add_attribute<VertexAttr::VEC2, 0, false, BufferUsage::STREAM>(
            const ArrayBuffer<BufferUsage::STREAM> &array_buffer);

    template void VertexArray::add_attribute<VertexAttr::VEC2, 1, false, BufferUsage::STREAM>(
            const ArrayBuffer<BufferUsage::STREAM> &array_buffer);

    template void VertexArray::add_attribute<VertexAttr::VEC4, 1, false, BufferUsage::STREAM>(
            const ArrayBuffer<BufferUsage::STREAM> &array_buffer);

}