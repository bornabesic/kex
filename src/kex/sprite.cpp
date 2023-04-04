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

#include <kex/sprite.hpp>
#include <cmath>

namespace kex {

    class Sprite::Impl {
    public:
        explicit Impl(const std::shared_ptr<Texture> texture) :
                Impl(texture, {0, 0, texture->width(), texture->height()}) {}

        explicit Impl(const std::shared_ptr<Texture> &texture, const RectangleDef &rectangle) :
                texture(texture),
                texture_region(rectangle),
                u_min(compute_u_min(rectangle, texture)),
                u_max(compute_u_max(rectangle, texture)),
                v_min(compute_v_min(rectangle, texture)),
                v_max(compute_v_max(rectangle, texture)),
                w(rectangle.w),
                h(rectangle.h) {}


        [[nodiscard]] std::array<float, 3 * 3>
        get_transform(float x, float y, float rotation, float shear_x, float shear_y, float scale_x,
                      float scale_y) const {
            // NOTE Column-major!
            // Vertex is a column vector multiplied by the transform from the left

            // 1. Rotation
            // {
            //     cos, sin, 0,
            //     -sin, cos 0,
            //     0, 0, 1
            // };

            // 2. Translation
            // {
            //     w, shear_y, 0,
            //     shear_x, h, 0,
            //     x, y, 1
            // };

            // Negative to keep CCW rotation direction since y-axis points down in the pixel coordinate system
            const auto cos = std::cos(-rotation);
            const auto sin = std::sin(-rotation);
            const auto w_scaled = w * scale_x;
            const auto h_scaled = h * scale_y;
            return {
                    w_scaled * cos + shear_x * sin, shear_y * cos + h_scaled * sin, 0,
                    -w_scaled * sin + shear_x * cos, -shear_y * sin + h_scaled * cos, 0,
                    x, y, 1,
            };
        }

    private:
        const std::shared_ptr<Texture> texture;
        const RectangleDef texture_region;
        const float u_min, u_max, v_min, v_max;
        const float w, h;

        static inline float compute_u_min(const RectangleDef &region, const std::shared_ptr<Texture> texture) {
            return static_cast<float>(region.x) / texture->width();
        }

        static inline float compute_u_max(const RectangleDef &region, const std::shared_ptr<Texture> texture) {
            return static_cast<float>(region.x + region.w) / texture->width();
        }

        static inline float compute_v_min(const RectangleDef &region, const std::shared_ptr<Texture> texture) {
            return 1 - static_cast<float>(region.y + region.h) / texture->height();
        }

        static inline float compute_v_max(const RectangleDef &region, const std::shared_ptr<Texture> texture) {
            return 1 - static_cast<float>(region.y) / texture->height();
        }

        friend Sprite;
    };

    Sprite::Sprite(const std::shared_ptr<Texture> texture) : impl(std::make_unique<Impl>(texture)) {}

    Sprite::Sprite(const std::shared_ptr<Texture> texture, const RectangleDef &region) : impl(
            std::make_unique<Impl>(texture, region)) {}

    std::shared_ptr<Texture> Sprite::texture() const { return impl->texture; }

    const RectangleDef &Sprite::texture_region() const { return impl->texture_region; }

    float Sprite::u_min() const { return impl->u_min; }

    float Sprite::u_max() const { return impl->u_max; }

    float Sprite::v_min() const { return impl->v_min; }

    float Sprite::v_max() const { return impl->v_max; }

    int Sprite::width() const { return impl->w; }

    int Sprite::height() const { return impl->h; }

    std::array<float, 3 * 3> Sprite::transform() const {
        return impl->get_transform(x, y, rotation, shear_x, shear_y, scale_x, scale_y);
    }

    void Sprite::set_tint(float r, float g, float b, float a) {
        tint_r = r;
        tint_g = g;
        tint_b = b;
        tint_a = a;
    }

    void Sprite::set_position(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void Sprite::set_scale(float scale_x, float scale_y) {
        this->scale_x = scale_x;
        this->scale_y = scale_y;
    }

    void Sprite::set_shear(float shear_x, float shear_y) {
        this->shear_x = shear_x;
        this->shear_y = shear_y;
    }

    Sprite::Sprite(Sprite &&other) noexcept = default;

    Sprite::~Sprite() = default;

}
