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

#include <unordered_map>
#include <kex/spritebatch.h>
#include <kex/sprite.h>
#include <kex/shader.h>
#include <kex/kex.h>
#include <vector>

namespace kex {

    class SpriteBatch::Impl {
    public:
        Impl() = default;

        void add(const Sprite &sprite) {
            const auto &texture = sprite.get_texture();
            groups[texture.get_id()].push_back(&sprite);
        }

        ~Impl() {
            for (const auto &[texture_id, sprites]: groups) {
                const auto &texture = sprites[0]->get_texture();

                std::vector<float> positions_data;
                std::vector<float> texture_coords_data;
                positions_data.reserve(sprites.size() * 4 * 2);
                texture_coords_data.reserve(sprites.size() * 4 * 2);
                for (const auto *sprite: sprites) {
                    const auto region = sprite->get_texture_region();
                    positions_data.insert(
                            positions_data.end(),
                            {
                                    static_cast<float>(region.x), static_cast<float>(region.y + region.h),
                                    static_cast<float>(region.x + region.w), static_cast<float>(region.y + region.h),
                                    static_cast<float>(region.x), static_cast<float>(region.y),
                                    static_cast<float>(region.x + region.w), static_cast<float>(region.y)
                            }
                    );
                    texture_coords_data.insert(
                            texture_coords_data.end(),
                            {
                                    sprite->get_u_min(), sprite->get_v_min(),
                                    sprite->get_u_max(), sprite->get_v_min(),
                                    sprite->get_u_max(), sprite->get_v_max(),
                                    sprite->get_u_min(), sprite->get_v_max(),
                            }
                    );
                }

                glBindBuffer(GL_ARRAY_BUFFER, sprite_buffers.positions);
                glBufferData(GL_ARRAY_BUFFER, max_sprite_instances * 4 * 2 * sizeof(float), nullptr,
                             GL_STREAM_DRAW); // Orphan
                glBufferSubData(GL_ARRAY_BUFFER, 0, positions_data.size() * sizeof(float), positions_data.data());

                glBindBuffer(GL_ARRAY_BUFFER, sprite_buffers.tex_coords);
                glBufferData(GL_ARRAY_BUFFER, max_sprite_instances * 4 * 2 * sizeof(float), nullptr,
                             GL_STREAM_DRAW); // Orphan
                glBufferSubData(GL_ARRAY_BUFFER, 0, texture_coords_data.size() * sizeof(float), texture_coords_data.data());

                texture.bind();
                glDrawArraysInstanced(
                        GL_TRIANGLE_STRIP,
                        0, 4, sprites.size() // NOLINT(cppcoreguidelines-narrowing-conversions)
                );
            }
        }

    private:
        std::unordered_map<unsigned int, std::vector<const Sprite *>> groups;

        friend SpriteBatch;
    };

    SpriteBatch::SpriteBatch() : impl(std::make_unique<Impl>()) {}

    SpriteBatch::~SpriteBatch() = default;

    void SpriteBatch::add(const Sprite &sprite) { impl->add(sprite); }
}
