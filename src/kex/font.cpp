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

#include <stdexcept>
#include <kex/font.hpp>
#include <kex/texture.hpp>
#include <kex/utils.hpp>
#include <kex/sprite.hpp>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace kex {

    class Font::Impl {
    public:
        Impl(const std::string &ttf_path, int size, int unicode_start, int unicode_end, unsigned int oversampling)
                : unicode_start(unicode_start), unicode_end(unicode_end) {
            constexpr int padding = 1;

            bitmap.resize(BITMAP_SIZE * BITMAP_SIZE);

            const int unicode_count = unicode_end - unicode_start + 1;

            const auto ttf_bytes = read_file(ttf_path);
            const auto *ttf_bytes_ptr = reinterpret_cast<const unsigned char *>(ttf_bytes.data());

            stbtt_fontinfo font_info;
            stbtt_InitFont(&font_info, ttf_bytes_ptr, 0);

            stbtt_pack_context ctx;
            auto success = stbtt_PackBegin(&ctx, bitmap.data(), BITMAP_SIZE, BITMAP_SIZE, 0, padding,
                                           nullptr);
            if (!success) {
                throw std::runtime_error("Could not load the font.");
            }

            stbtt_PackSetOversampling(&ctx, oversampling, oversampling);

            packed_chars.resize(unicode_count);
            success = stbtt_PackFontRange(&ctx, ttf_bytes_ptr, 0, size, unicode_start, unicode_count,
                                          packed_chars.data());
            if (!success) {
                throw std::runtime_error("Could not pack the font.");
            }

            // glyph_indices.reserve(unicode_count);
            // for (int codepoint = unicode_start; codepoint <= unicode_end; ++codepoint) {
            //     const auto glyph_index = stbtt_FindGlyphIndex(&font_info, codepoint);
            //     glyph_indices.push_back(glyph_index);
            // }

            stbtt_PackEnd(&ctx);

            texture = std::make_shared<Texture>(bitmap, BITMAP_SIZE, BITMAP_SIZE, PixelFormat::LUMINANCE, true);
        }

        Text make(const std::string &text) {
            const auto codepoints = get_codepoints_from_utf8(text);

            stbtt_aligned_quad quad;
            std::vector<Sprite> char_sprites;
            char_sprites.reserve(codepoints.size());
            for (const auto codepoint: codepoints) {
                float x = 0;
                float y = 0;
                stbtt_GetPackedQuad(packed_chars.data(), BITMAP_SIZE, BITMAP_SIZE, codepoint - unicode_start, &x, &y,
                                    &quad, true);

                const int region_x = quad.s0 * BITMAP_SIZE;
                const int region_y = quad.t0 * BITMAP_SIZE;
                const int region_w = (quad.s1 - quad.s0) * BITMAP_SIZE;
                const int region_h = (quad.t1 - quad.t0) * BITMAP_SIZE;
                const RectangleDef region = {region_x, region_y,
                                             region_w, region_h};
                char_sprites.emplace_back(texture, region);
            }
            return Text(text, std::move(char_sprites));
        }

    private:
        static constexpr int BITMAP_SIZE = 512;

        int unicode_start;
        int unicode_end;
        std::vector<unsigned char> bitmap;
        std::vector<stbtt_packedchar> packed_chars;
        std::shared_ptr<Texture> texture;
        // std::vector<int> glyph_indices;

        friend Font;
    };

    Font::Font(const std::string &ttf_path, int size, int unicode_start, int unicode_end, unsigned int oversampling)
            : impl(
            std::make_unique<Impl>(ttf_path, size, unicode_start, unicode_end, oversampling)) {}

    Text Font::make(const std::string &text) const { return impl->make(text); }

    const std::shared_ptr<Texture> Font::texture() {
        return impl->texture;
    }

    Font::~Font() = default;

}
