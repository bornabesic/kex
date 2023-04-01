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

#include <kex/buffer.hpp>

#ifdef KEX_USE_GLEW
#include <GL/glew.h>
#endif

namespace kex {

    template<BufferType T>
    struct BufferState {
        static GLuint bound_id;
    };

    template<BufferType T>
    GLuint BufferState<T>::bound_id = 0;

    template<BufferType T, BufferUsage U>
    class Buffer<T, U>::Impl {
    public:
        explicit Impl(int size) : Impl() {
            bsize = size;
            glBufferData(target, size, nullptr, usage);
        }

        Impl() {
            glGenBuffers(1, &id);
            if constexpr (T == BufferType::ARRAY) {
                target = GL_ARRAY_BUFFER;
            }

            if constexpr (U == BufferUsage::STATIC) {
                usage = GL_STATIC_DRAW;
            } else if constexpr (U == BufferUsage::STREAM) {
                usage = GL_STREAM_DRAW;
            }
        }

        void replace(const void *data, int size) {
            this->bind();
            glBufferData(target, size, data, usage);
        }

        void update(const void *data, int size, int offset) {
            this->bind();
            glBufferSubData(target, offset, size, data);
        }

        void orphan(int size) {
            this->bind();
            glBufferData(target, size == -1 ? bsize : size, nullptr, usage);
        }

        void bind() const {
            if (BufferState<T>::bound_id == id) return;

            glBindBuffer(target, id);
            BufferState<T>::bound_id = id;
        }

        void unbind() const {
            if (BufferState<T>::bound_id != id) return;
            glBindBuffer(target, 0);
            BufferState<T>::bound_id = 0;
        }

        [[nodiscard]] bool is_bound() const {
            return BufferState<T>::bound_id == id;
        }

        ~Impl() {
            glDeleteBuffers(1, &id);
        }

    private:
        GLenum target = 0;
        GLenum usage = 0;
        GLuint id = 0;
        int bsize = 0;

        friend Buffer<T, U>;
    };

    template<BufferType T, BufferUsage U>
    Buffer<T, U>::Buffer() : impl(std::make_unique<Impl>()) {}

    template<BufferType T, BufferUsage U>
    Buffer<T, U>::Buffer(int size) : impl(std::make_unique<Impl>(size)) {}

    template<BufferType T, BufferUsage U>
    Buffer<T, U>::Buffer(Buffer<T, U> &&buffer) noexcept = default;

    template<BufferType T, BufferUsage U>
    constexpr BufferType Buffer<T, U>::type() const { return T; }

    template<BufferType T, BufferUsage U>
    constexpr BufferUsage Buffer<T, U>::usage() const { return U; }

    template<BufferType T, BufferUsage U>
    unsigned int Buffer<T, U>::id() const { return impl->id; }

    template<BufferType T, BufferUsage U>
    void Buffer<T, U>::replace(const void *data, int size) { impl->replace(data, size); }

    template<BufferType T, BufferUsage U>
    void Buffer<T, U>::update(const void *data, int size, int offset) { impl->update(data, size, offset); }

    template<BufferType T, BufferUsage U>
    void Buffer<T, U>::bind() const { impl->bind(); }

    template<BufferType T, BufferUsage U>
    void Buffer<T, U>::unbind() const { impl->unbind(); }

    template<BufferType T, BufferUsage U>
    bool Buffer<T, U>::is_bound() const { return impl->is_bound(); }

    template<BufferType T, BufferUsage U>
    void Buffer<T, U>::orphan(int size) { impl->orphan(size); }

    template<BufferType T, BufferUsage U>
    Buffer<T, U>::~Buffer() = default;

    // Specializations
    template
    class Buffer<BufferType::ARRAY, BufferUsage::STATIC>;

    template
    class Buffer<BufferType::ARRAY, BufferUsage::STREAM>;

}