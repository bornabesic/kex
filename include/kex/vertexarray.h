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

#ifndef KEX_VERTEXARRAY_H
#define KEX_VERTEXARRAY_H

#include <memory>
#include <kex/buffer.h>

namespace kex {

    enum VertexAttr {
        VEC2,
        VEC4,
        MAT3,
    };

    class VertexArray {
    public:
        VertexArray();

        VertexArray(VertexArray &&vertex_array) noexcept;

        [[nodiscard]] unsigned int get_id() const;

        void bind() const;

        template<VertexAttr Attr, int Div = 0, bool Norm = false, BufferUsage Usg>
        void add_attribute(const ArrayBuffer<Usg> &array_buffer);

        ~VertexArray();

    private:
        class Impl;

        std::unique_ptr<Impl> impl;
    };

}

#endif //KEX_VERTEXARRAY_H
