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
#include <cmath>

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
                v_max(compute_v_max(rectangle, texture)),
                w(rectangle.w),
                h(rectangle.h) {}

        void set_color(float red, float green, float blue, float alpha) {
            r = red;
            g = green;
            b = blue;
            a = alpha;
        }

        [[nodiscard]] std::array<float, 3 * 3> get_transform() const {
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

            const auto cos = std::cos(rotation);
            const auto sin = std::sin(rotation);
            const auto w_scaled = w * scale_x;
            const auto h_scaled = h * scale_y;
            return {
                    w_scaled * cos + shear_x * sin, shear_y * cos + h_scaled * sin, 0,
                    -w_scaled * sin + shear_x * cos, -shear_y * sin + h_scaled * cos, 0,
                    x, y, 1,
            };
        }

    private:
        const Texture &texture;
        const RectangleDef texture_region;
        const float u_min, u_max, v_min, v_max;
        const float w, h;
        float x = 0, y = 0;
        float r = 1.f, g = 1.f, b = 1.f, a = 0.f;
        float scale_x = 1.f, scale_y = 1.f;
        float shear_x = 0.f, shear_y = 0.f;
        float rotation = 0.f;

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

    int Sprite::get_width() const { return impl->w; }

    int Sprite::get_height() const { return impl->h; }

    float Sprite::get_position_x() const { return impl->x; }

    float Sprite::get_position_y() const { return impl->y; }

    void Sprite::set_position_x(float x) { impl->x = x; }

    void Sprite::set_position_y(float y) { impl->y = y; }

    void Sprite::set_color(float red, float green, float blue, float alpha) {
        impl->set_color(red, green, blue, alpha);
    }

    float Sprite::get_color_red() const { return impl->r; }

    float Sprite::get_color_blue() const { return impl->b; }

    float Sprite::get_color_green() const { return impl->g; }

    float Sprite::get_color_alpha() const { return impl->a; }

    void Sprite::set_position_xy(float x, float y) {
        impl->x = x;
        impl->y = y;
    }

    std::array<float, 3 * 3> Sprite::get_transform() const { return impl->get_transform(); }

    float Sprite::get_shear_x() const { return impl->shear_x; }

    float Sprite::get_shear_y() const { return impl->shear_y; }

    void Sprite::set_shear_x(float shear_x) { impl->shear_x = shear_x; }

    void Sprite::set_shear_y(float shear_y) { impl->shear_y = shear_y; }

    void Sprite::set_shear_xy(float shear_x, float shear_y) {
        impl->shear_x = shear_x;
        impl->shear_y = shear_y;
    }

    void Sprite::set_rotation(float rotation) { impl->rotation = rotation; }

    float Sprite::get_rotation() const { return impl->rotation; }

    float Sprite::get_scale_x() const { return impl->scale_x; }

    float Sprite::get_scale_y() const { return impl->scale_y; }

    void Sprite::set_scale_x(float scale_x) { impl->scale_x = scale_x; }

    void Sprite::set_scale_y(float scale_y) { impl->scale_y = scale_y; }

    void Sprite::set_scale_xy(float scale_x, float scale_y) {
        impl->scale_x = scale_x;
        impl->scale_y = scale_y;
    }

    Sprite::~Sprite() = default;

}
