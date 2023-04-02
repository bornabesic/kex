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

#ifndef KEX_SPRITE_HPP
#define KEX_SPRITE_HPP

#include <memory>
#include <kex/texture.hpp>
#include <kex/def.hpp>

namespace kex {

    class Sprite {
    public:
        /** @name Translation
         */
        ///@{
        /** x-coordinate of the center of the sprite. */
        float x;

        /** y-coordinate of the center of the sprite. */
        float y;

        /**
         * Set the position of the sprite.
         *
         * This utility function is equal to
         * @code{.cpp}
         * // extern kex::Sprite sprite;
         * sprite.x = x;
         * sprite.y = y;
         * @endcode
         *
         * @param x x-coordinate of the sprite position
         * @param y y-coordinate of the sprite position
         */
        void set_position(float x, float y);
        ///@}

        /** @name Rotation
         */
        ///@{
        /** Counterclockwise (CCW) rotation around the center in radians. */
        float rotation = 0.f;
        ///@}

        /** @name Scale
         */
        ///@{
        /** Scale factor in the x-axis direction. */
        float scale_x = 1.f;

        /** Scale factor in the y-axis direction. */
        float scale_y = 1.f;

        /**
         * Set the scale of the sprite.
         *
         * This utility function is equal to
         * @code{.cpp}
         * // extern kex::Sprite sprite;
         * sprite.scale_x = scale_x;
         * sprite.scale_y = scale_y;
         * @endcode
         *
         * @param scale_x Scale factor in the x-axis direction
         * @param scale_y Scale factor in the y-axis direction
         */
        void set_scale(float scale_x, float scale_y);
        ///@}

        /** @name Shear
         */
        ///@{
        /** Shear factor in the x-axis direction. */
        float shear_x = 0.f;

        /** Shear factor in the y-axis direction. */
        float shear_y = 0.f;

        /**
         * Set the shear of the sprite.
         *
         * This utility function is equal to
         * @code{.cpp}
         * // extern kex::Sprite sprite;
         * sprite.shear_x = shear_x;
         * sprite.shear_y = shear_y;
         * @endcode
         *
         * @param shear_x Shear factor in the x-axis direction
         * @param shear_y Shear factor in the y-axis direction
         */
        void set_shear(float shear_x, float shear_y);
        ///@}

        /** @name Tint
         *  Sprites support tinting by multiplying the source texture color by the specified tint color.
         *  @verbatim embed:rst:leading-asterisk
         *  .. note::
         *     Currently, sprite's alpha can be manipulated by changing the alpha component of the tint color.
         *  @endverbatim
         */
        ///@{
        /** Red component of the tint color. */
        float tint_r = 1.f;

        /** Green component of the tint color. */
        float tint_g = 1.f;

        /** Blue component of the tint color. */
        float tint_b = 1.f;

        /** Alpha component of the tint color. */
        float tint_a = 1.f;

        /**
         * Set the tint color of the sprite.
         *
         * This utility function is equal to
         * @code{.cpp}
         * // extern kex::Sprite sprite;
         * sprite.tint_r = r;
         * sprite.tint_g = g;
         * sprite.tint_b = b;
         * sprite.tint_a = a;
         * @endcode
         *
         * @param r Red component of the tint color
         * @param g Greem component of the tint color
         * @param b Blue component of the tint color
         * @param a Alpha component of the tint color
         */
        void set_tint(float r, float g, float b, float a = 1.f);
        ///@}

        /**
         * Create a sprite from a texture, inheriting its width and height.
         *
         * @param texture Texture for the sprite
         */
        explicit Sprite(const Texture &texture);

        /**
         * Create a sprite from a texture region.
         *
         * @param texture Texture for the sprite
         * @param region Region of the texture used to render the sprite
         */
        Sprite(const Texture &texture, const RectangleDef &region);

        Sprite(Sprite &&) noexcept;

        ~Sprite();

        /** Width of the sprite. */
        [[nodiscard]] int width() const;

        /** Height of the sprite. */
        [[nodiscard]] int height() const;

        /** @name Transform matrix
         */
        ///@{
        /**
         * Retrieve a 3 x 3 column-major homogeneous transformation matrix in a flat array.
         * @return Matrix representing the geometrical transformation of the sprite
         */
        [[nodiscard]] std::array<float, 3 * 3> transform() const;
        ///@}

        /** @name Texture
         */
        ///@{
        /** Texture for the sprite. */
        [[nodiscard]] const Texture &texture() const;

        /** Region of the texture used for rendering the sprite. */
        [[nodiscard]] const RectangleDef &texture_region() const;

        /** Start u-coordinate of the region within the texture */
        [[nodiscard]] float u_min() const;

        /** End u-coordinate of the region within the texture */
        [[nodiscard]] float u_max() const;

        /** Start v-coordinate of the region within the texture */
        [[nodiscard]] float v_min() const;

        /** End v-coordinate of the region within the texture */
        [[nodiscard]] float v_max() const;
        ///@}

    private:
        class Impl;

        std::unique_ptr<Impl> impl;
    };

}

#endif //KEX_SPRITE_HPP
