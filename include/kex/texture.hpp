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

#ifndef KEX_TEXTURE_HPP
#define KEX_TEXTURE_HPP

#include <string>
#include <memory>

namespace kex {

    class Texture {
    public:
        /**
         * Load a texture from an image file.
         *
         * @param path Path to the texture image file
         * @param mipmap Flag indicating whether to generate a texture mipmap
         */
        explicit Texture(const std::string &path, bool mipmap = false);

        /**
         * Bind the current texture for rendering.
         */
        void bind() const;

        /**
         * Bind the texture with the specified identifier for rendering.
         */
        static void bind(unsigned int id);

        /** Width of the texture in pixels. */
        [[nodiscard]] int width() const;

        /** Height of the texture in pixels. */
        [[nodiscard]] int height() const;

        /** Texture identifier. */
        [[nodiscard]] unsigned int id() const;

        ~Texture();

    private:
        class Impl;

        std::unique_ptr<Impl> impl;
    };

} // kex

#endif //KEX_TEXTURE_HPP
