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

#ifndef KEX_SPRITE_H
#define KEX_SPRITE_H

#include <memory>
#include <kex/texture.h>
#include <kex/def.h>

namespace kex {

    class Sprite {
    public:
        explicit Sprite(const Texture &texture);

        explicit Sprite(const Texture &texture, const RectangleDef &region);

        ~Sprite();

        [[nodiscard]] const Texture &get_texture() const;

        [[nodiscard]] const RectangleDef &get_texture_region() const;

        [[nodiscard]] int get_width() const;

        [[nodiscard]] int get_height() const;

        [[nodiscard]] float get_position_x() const;

        [[nodiscard]] float get_position_y() const;

        void set_position_x(float x);

        void set_position_y(float y);

        [[nodiscard]] float get_u_min() const;

        [[nodiscard]] float get_u_max() const;

        [[nodiscard]] float get_v_min() const;

        [[nodiscard]] float get_v_max() const;

    private:
        class Impl;

        std::unique_ptr<Impl> impl;
    };

}

#endif //KEX_SPRITE_H
