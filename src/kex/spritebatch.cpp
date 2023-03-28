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

#ifdef KEX_USE_GLEW
#include <GL/glew.h>
#endif

namespace kex {

    class SpriteBatch::Impl {
    public:
        Impl() = default;

        void add(const Sprite &sprite) {
            const auto &texture = sprite.get_texture();
            groups[texture.get_id()].push_back(&sprite);
        }

        ~Impl() {
            std::vector<float> v_tex_coords;
            std::vector<float> s_positions;
            std::vector<float> s_sizes;

            for (const auto &[texture_id, sprites]: groups) {
                const auto &texture = sprites[0]->get_texture();

                v_tex_coords.clear();
                s_positions.clear();

                v_tex_coords.reserve(sprites.size() * 4 * 2);
                s_positions.reserve(sprites.size() * 2);
                s_sizes.reserve(sprites.size() * 2);
                for (const auto *sprite: sprites) {
                    s_positions.insert(
                            s_positions.end(),
                            {
                                    static_cast<float>(sprite->get_x()),
                                    static_cast<float>(sprite->get_y())
                            }
                    );
                    s_sizes.insert(
                            s_sizes.end(),
                            {
                                    static_cast<float>(sprite->get_width()),
                                    static_cast<float>(sprite->get_height()),
                            }
                    );
                    v_tex_coords.insert(
                            v_tex_coords.end(),
                            {
                                    sprite->get_u_min(), sprite->get_v_min(),
                                    sprite->get_u_max(), sprite->get_v_min(),
                                    sprite->get_u_min(), sprite->get_v_max(),
                                    sprite->get_u_max(), sprite->get_v_max(),
                            }
                    );
                }

                glBindBuffer(GL_ARRAY_BUFFER, sprite_buffers.s_positions);
                glBufferData(GL_ARRAY_BUFFER, sprites.size() * 4 * 2 * sizeof(float), nullptr,
                             GL_STREAM_DRAW); // Orphan
                glBufferSubData(GL_ARRAY_BUFFER, 0, s_positions.size() * sizeof(float), s_positions.data());

                glBindBuffer(GL_ARRAY_BUFFER, sprite_buffers.s_sizes);
                glBufferData(GL_ARRAY_BUFFER, sprites.size() * 4 * 2 * sizeof(float), nullptr,
                             GL_STREAM_DRAW); // Orphan
                glBufferSubData(GL_ARRAY_BUFFER, 0, s_sizes.size() * sizeof(float), s_sizes.data());

                glBindBuffer(GL_ARRAY_BUFFER, sprite_buffers.v_tex_coords);
                glBufferData(GL_ARRAY_BUFFER, sprites.size() * 4 * 2 * sizeof(float), nullptr,
                             GL_STREAM_DRAW); // Orphan
                glBufferSubData(GL_ARRAY_BUFFER, 0, v_tex_coords.size() * sizeof(float), v_tex_coords.data());

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
