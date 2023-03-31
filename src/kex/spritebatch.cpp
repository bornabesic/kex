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
        layout (location = 2) in highp mat3 transform_in;
        // layout (location = 3)
        // layout (location = 4)
        layout (location = 5) in highp vec4 tint_in;

        out highp vec2 tex_coords;
        out highp vec4 tint;

        void main() {
            highp vec3 position = transform_in * vec3(base_position_in, 1) / vec3(800, 600, 1);
            gl_Position = vec4(position.xy, 0, 1);
            tex_coords = tex_coords_in;
            tint = tint_in;
        }
    )";

    static constexpr auto fragment_shader_source = R"(
        #version 300 es

        uniform sampler2D tex_u;

        in highp vec2 tex_coords;
        in highp vec4 tint;

        out highp vec4 color_out;

        void main() {
            color_out = texture(tex_u, tex_coords) * tint;
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
        StreamArrayBuffer s_transforms;
        StreamArrayBuffer s_tints;
    };

    struct SpriteBatchGroupData {
        unsigned int texture_id = 0;
        std::vector<float> v_tex_coords;
        std::vector<float> s_transforms;
        std::vector<float> s_tints;
        int instance_count = 0;
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
                ctx.vao.add_attribute<VertexAttr::MAT3, 1>(ctx.s_transforms);
                ctx.vao.add_attribute<VertexAttr::VEC4, 1>(ctx.s_tints);
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
            auto &group_data = groups[texture.get_id()];
            group_data.texture_id = texture.get_id();
            const auto transform = sprite.get_transform();
            group_data.s_transforms.insert(
                    group_data.s_transforms.end(),
                    transform.begin(), transform.end()
            );
            group_data.v_tex_coords.insert(
                    group_data.v_tex_coords.end(),
                    {
                            sprite.u_min(), sprite.v_min(),
                            sprite.u_max(), sprite.v_min(),
                            sprite.u_min(), sprite.v_max(),
                            sprite.u_max(), sprite.v_max(),
                    }
            );
            group_data.s_tints.insert(
                    group_data.s_tints.end(),
                    {
                            sprite.tint_r,
                            sprite.tint_g,
                            sprite.tint_b,
                            sprite.tint_a,
                    }
            );

            ++group_data.instance_count;
        }

        ~Impl() {

            for (const auto &[texture_id, data]: groups) {
                auto &ctx = Impl::ctxs[ctx_index];
                ctx.v_tex_coords.orphan(data.instance_count * 4 * 2 * sizeof(float));
                ctx.v_tex_coords.update(data.v_tex_coords.data(), data.v_tex_coords.size() * sizeof(float));

                ctx.s_transforms.orphan(data.instance_count * 3 * 3 * sizeof(float));
                ctx.s_transforms.update(data.s_transforms.data(), data.s_transforms.size() * sizeof(float));

                ctx.s_tints.orphan(data.instance_count * 4 * sizeof(float));
                ctx.s_tints.update(data.s_tints.data(), data.s_tints.size() * sizeof(float));

                ctx.vao.bind();
                Texture::bind(data.texture_id);
                glDrawArraysInstanced(
                        GL_TRIANGLE_STRIP,
                        0, 4, data.instance_count // NOLINT(cppcoreguidelines-narrowing-conversions)
                );

                --last_used_ctx_index;
            }
        }

    private:
        std::unordered_map<unsigned int, SpriteBatchGroupData> groups;
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
