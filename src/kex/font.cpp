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
#include <kex/utils.hpp>

#define STB_TRUETYPE_IMPLEMENTATION

#include <stb_truetype.h>

namespace kex {

    class Font::Impl {
    public:
        Impl(const std::string &ttf_path, int size, int unicode_start, int unicode_end, unsigned int oversampling) {
            constexpr int padding = 1;
            const int bitmap_size = 512; // TODO Move
            bitmap.resize(bitmap_size * bitmap_size);

            const int unicode_count = unicode_end - unicode_start + 1;

            const auto ttf_bytes = read_file(ttf_path);
            const auto *ttf_bytes_ptr = reinterpret_cast<const unsigned char *>(ttf_bytes.data());

            stbtt_fontinfo font_info;
            stbtt_InitFont(&font_info, ttf_bytes_ptr, 0);

            stbtt_pack_context ctx;
            auto success = stbtt_PackBegin(&ctx, bitmap.data(), bitmap_size, bitmap_size, 0, padding,
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

            stbtt_PackEnd(&ctx);
        }

        Text make(const std::string &text) {
            // TODO
            return Text(text);
        }

    private:
        std::vector<unsigned char> bitmap;
        std::vector<stbtt_packedchar> packed_chars;
    };

    Font::Font(const std::string &ttf_path, int size, int unicode_start, int unicode_end, unsigned int oversampling)
            : impl(
            std::make_unique<Impl>(ttf_path, size, unicode_start, unicode_end, oversampling)) {}

    Text Font::make(const std::string &text) const { return impl->make(text); }

    Font::~Font() = default;

}
