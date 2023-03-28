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

#ifndef KEX_BUFFER_H
#define KEX_BUFFER_H

#include <memory>

namespace kex {

    enum BufferType {
        ARRAY,
    };

    enum BufferUsage {
        STATIC,
        STREAM,
    };

    template<BufferType T, BufferUsage U>
    class Buffer {
    public:
        explicit Buffer(int size);

        explicit Buffer();

        [[nodiscard]] constexpr BufferType get_type() const;

        [[nodiscard]] constexpr BufferUsage get_usage() const;

        void replace(const void *data, int size);

        void update(const void *data, int size, int offset = 0);

        void bind() const;

        void orphan(int size = -1);

    private:
        class Impl;

        std::unique_ptr<Impl> impl;
    };

    template<BufferUsage U>
    using ArrayBuffer = Buffer<BufferType::ARRAY, U>;

    using StaticArrayBuffer = ArrayBuffer<BufferUsage::STATIC>;
    using StreamArrayBuffer = ArrayBuffer<BufferUsage::STREAM>;

}


#endif //KEX_BUFFER_H
