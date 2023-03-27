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
#include <kex/shader.h>
#include <kex/program.h>
#include <vector>

#ifdef KEX_USE_GLEW

#include <GL/glew.h>

#endif

namespace kex {

    class SpriteBatch::Impl {
    public:
        Impl() :
                vertex_shader(SpriteBatch::Impl::vertex_shader_source),
                fragment_shader(SpriteBatch::Impl::fragment_shader_source),
                program(vertex_shader, fragment_shader) {
            static float quad_data[] = {
                    -1.f, -1.f,
                    1.f, -1.f,
                    1.f, 1.f,
                    -1.f, 1.f,
            };
            glGenBuffers(1, &quad_buffer); // TODO Call once
            glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
            glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), quad_data, GL_STATIC_DRAW);
        }

        void add(const Sprite &sprite) {
            const auto &texture = sprite.get_texture();
            groups[texture.get_id()].push_back(&sprite);
        }

        ~Impl() {
            program.use();
            for (const auto &[texture_id, sprites]: groups) {
                const auto &texture = sprites[0]->get_texture();

                std::vector<float> texture_coordinates_data;
                texture_coordinates_data.reserve(sprites.size() * 4 * 2);
                for (const auto *sprite: sprites) {
                    texture_coordinates_data.insert(
                            texture_coordinates_data.end(),
                            {
                                    sprite->get_u_min(), sprite->get_v_min(),
                                    sprite->get_u_max(), sprite->get_v_min(),
                                    sprite->get_u_max(), sprite->get_v_max(),
                                    sprite->get_u_min(), sprite->get_v_max(),
                            }
                    );
                }

                texture.bind();
                // TODO Draw
            }

            glDeleteBuffers(1, &quad_buffer); // TODO Call once
        }

    private:
        GLuint quad_buffer = 0;
        std::unordered_map<unsigned int, std::vector<const Sprite *>> groups;

        // TODO Move into global state
        VertexShader vertex_shader;
        FragmentShader fragment_shader;
        Program program;

        static constexpr auto vertex_shader_source = R"(
            #version 300 es

            void main() {
                gl_Position = vec4(0, 0, 0, 0);
            }
        )";

        static constexpr auto fragment_shader_source = R"(
            #version 300 es

            out highp vec4 color;

            void main() {
                color = vec4(1, 0, 0, 1);
            }
        )";

        friend SpriteBatch;
    };

    SpriteBatch::SpriteBatch() : impl(std::make_unique<Impl>()) {}

    SpriteBatch::~SpriteBatch() = default;

    void SpriteBatch::add(const Sprite &sprite) { impl->add(sprite); }
}
