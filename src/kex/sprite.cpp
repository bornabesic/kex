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
        explicit Impl(const Texture &texture) :
                Impl(texture, {0, 0, texture.get_width(), texture.get_height()}) {}

        explicit Impl(const Texture &texture, const RectangleDef &rectangle) :
                texture(texture),
                texture_region(rectangle),
                u_min(compute_u_min(rectangle, texture)),
                u_max(compute_u_max(rectangle, texture)),
                v_min(compute_v_min(rectangle, texture)),
                v_max(compute_v_max(rectangle, texture)) {}

    private:
        const Texture &texture;
        const RectangleDef texture_region;
        const float u_min, u_max, v_min, v_max;

        static inline float compute_u_min(const RectangleDef &region, const Texture &texture) {
            return static_cast<float>(region.x) / texture.get_width();
        }

        static inline float compute_u_max(const RectangleDef &region, const Texture &texture) {
            return static_cast<float>(region.x + region.w) / texture.get_width();
        }

        static inline float compute_v_min(const RectangleDef &region, const Texture &texture) {
            return 1 - static_cast<float>(region.y + region.h) / texture.get_height();
        }

        static inline float compute_v_max(const RectangleDef &region, const Texture &texture) {
            return 1 - static_cast<float>(region.y) / texture.get_height();
        }

        friend Sprite;
    };

    Sprite::Sprite(const kex::Texture &texture) : impl(std::make_unique<Impl>(texture)) {}

    Sprite::Sprite(const Texture &texture, const RectangleDef &region) : impl(
            std::make_unique<Impl>(texture, region)) {}

    const Texture &Sprite::get_texture() const { return impl->texture; }

    const RectangleDef &Sprite::get_texture_region() const { return impl->texture_region; }

    float Sprite::get_u_min() const { return impl->u_min; }

    float Sprite::get_u_max() const { return impl->u_max; }

    float Sprite::get_v_min() const { return impl->v_min; }

    float Sprite::get_v_max() const { return impl->v_max; }

    Sprite::~Sprite() = default;

}
