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

#include <vector>
#include <unordered_map>
#include <kex/spritebatch.h>
#include <kex/sprite.h>
#include <kex/shader.h>
#include <kex/kex.h>
#include <kex/program.h>
#include <kex/vertexarray.h>

#ifdef KEX_USE_GLEW
#include <GL/glew.h>
#endif

namespace kex {

    static constexpr auto vertex_shader_source = R"(
        #version 300 es

        layout (location = 0) in highp vec2 base_position_in;
        layout (location = 1) in highp vec2 tex_coords_in;
        layout (location = 2) in highp vec2 position_in;
        layout (location = 3) in highp vec2 size_in;

        out highp vec2 tex_coords;

        void main() {
            highp vec2 position = (base_position_in * size_in + position_in) / vec2(800, 600);
            gl_Position = vec4(position, 0, 1);
            tex_coords = tex_coords_in;
        }
    )";

    static constexpr auto fragment_shader_source = R"(
        #version 300 es

        uniform sampler2D tex_u;

        in highp vec2 tex_coords;

        out highp vec4 color_out;

        void main() {
            color_out = texture(tex_u, tex_coords);
        }
    )";

    static constexpr float normalized_positions_data[] = {
            -1.f, -1.f,
            1.f, -1.f,
            -1.f, 1.f,
            1.f, 1.f,
    };

    struct SpriteBatchCtx {
        VertexArray vao;
        StaticArrayBuffer v_positions{4 * 2 * sizeof(float)};
        StreamArrayBuffer v_tex_coords;
        StreamArrayBuffer s_positions;
        StreamArrayBuffer s_sizes;
    };

    class SpriteBatch::Impl {
    public:
        Impl() {
            ctx_index = last_used_ctx_index + 1;
            if (last_used_ctx_index == ctxs.size() - 1) {
                auto &ctx = Impl::ctxs.emplace_back();

                // Initialize the quad buffer
                ctx.v_positions.replace(normalized_positions_data, 4 * 2 * sizeof(float));

                // Initialize vertex attributes
                ctx.vao.add_attribute<VertexAttr::VEC2>(ctx.v_positions);
                ctx.vao.add_attribute<VertexAttr::VEC2>(ctx.v_tex_coords);
                ctx.vao.add_attribute<VertexAttr::VEC2, 1>(ctx.s_positions);
                ctx.vao.add_attribute<VertexAttr::VEC2, 1>(ctx.s_sizes);
            }
            ++last_used_ctx_index;

            // Shaders
            static auto vertex_shader = VertexShader(vertex_shader_source);
            static auto fragment_shader = FragmentShader(fragment_shader_source);
            static auto program = Program(vertex_shader, fragment_shader);
            static auto _ = []() {
                GLint texture_location = program.get_uniform_location("tex_u");
                glUniform1i(texture_location, Impl::TEXTURE_SLOT);
                return true;
            }();

            program.use();
        };

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
                s_sizes.clear();

                v_tex_coords.reserve(sprites.size() * 4 * 2);
                s_positions.reserve(sprites.size() * 2);
                s_sizes.reserve(sprites.size() * 2);
                for (const auto *sprite: sprites) {
                    s_positions.insert(
                            s_positions.end(),
                            {
                                    static_cast<float>(sprite->get_position_x()),
                                    static_cast<float>(sprite->get_position_y())
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

                auto &ctx = Impl::ctxs[ctx_index];
                ctx.v_tex_coords.orphan(sprites.size() * 4 * 2 * sizeof(float));
                ctx.v_tex_coords.update(v_tex_coords.data(), v_tex_coords.size() * sizeof(float));

                ctx.s_positions.orphan(sprites.size() * 2 * sizeof(float));
                ctx.s_positions.update(s_positions.data(), s_positions.size() * sizeof(float));

                ctx.s_sizes.orphan(sprites.size() * 2 * sizeof(float));
                ctx.s_sizes.update(s_sizes.data(), s_sizes.size() * sizeof(float));

                ctx.vao.bind();
                texture.bind();
                glDrawArraysInstanced(
                        GL_TRIANGLE_STRIP,
                        0, 4, sprites.size() // NOLINT(cppcoreguidelines-narrowing-conversions)
                );

                --last_used_ctx_index;
            }
        }

    private:
        std::unordered_map<unsigned int, std::vector<const Sprite *>> groups;
        int ctx_index = -1;

        static const int TEXTURE_SLOT = 0;

        static std::vector<SpriteBatchCtx> ctxs;
        static int last_used_ctx_index;

        friend SpriteBatch;
    };

    std::vector<SpriteBatchCtx> SpriteBatch::Impl::ctxs;
    int SpriteBatch::Impl::last_used_ctx_index = -1;

    SpriteBatch::SpriteBatch() : impl(std::make_unique<Impl>()) {}

    SpriteBatch::~SpriteBatch() = default;

    void SpriteBatch::add(const Sprite &sprite) { impl->add(sprite); }
}
