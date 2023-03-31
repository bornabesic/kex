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
        // Translation
        float x;
        float y;

        // Rotation
        float rotation = 0.f;

        // Scale
        float scale_x = 1.f;
        float scale_y = 1.f;

        // Shear
        float shear_x = 0.f;
        float shear_y = 0.f;

        // Tint
        float tint_r = 1.f;
        float tint_g = 1.f;
        float tint_b = 1.f;
        float tint_a = 1.f;

        explicit Sprite(const Texture &texture);

        explicit Sprite(const Texture &texture, const RectangleDef &region);

        ~Sprite();

        [[nodiscard]] const Texture &get_texture() const;

        [[nodiscard]] const RectangleDef &get_texture_region() const;

        [[nodiscard]] int width() const;

        [[nodiscard]] int height() const;

        void set_position(float x, float y);

        void set_scale(float scale_x, float scale_y);

        void set_shear(float shear_x, float shear_y);

        void set_tint(float r, float g, float b, float a = 1.f);

        [[nodiscard]] std::array<float, 3 * 3> get_transform() const;

        [[nodiscard]] float u_min() const;

        [[nodiscard]] float u_max() const;

        [[nodiscard]] float v_min() const;

        [[nodiscard]] float v_max() const;

    private:
        class Impl;

        std::unique_ptr<Impl> impl;
    };

}

#endif //KEX_SPRITE_H
