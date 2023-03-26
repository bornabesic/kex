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

#include <kex/sprite.h>

namespace kex {

    class Sprite::Impl {
    public:
        explicit Impl(const Texture &texture) : texture(texture) {}

        explicit Impl(const Texture &texture, int x, int y, int w, int h) : texture(texture), x(x), y(y), w(w), h(h) {}

    private:
        int x, y, w, h;
        const Texture &texture;

        friend Sprite;
    };

    Sprite::Sprite(const kex::Texture &texture) : impl(std::make_unique<Impl>(texture)) {}

    Sprite::Sprite(const Texture &texture, int x, int y, int w, int h) : impl(
            std::make_unique<Impl>(texture, x, y, w, h)) {}

    Sprite::~Sprite() = default;

}
