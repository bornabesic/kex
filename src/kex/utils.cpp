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

#include <kex/utils.hpp>
#include <vector>
#include <string>
#include <fstream>

namespace kex {

    std::vector<char> read_file(const std::string &path) {
        std::ifstream input_stream(path, std::ios::binary);
        input_stream.seekg(0, std::ios::end);
        const auto file_size = input_stream.tellg();
        input_stream.seekg(0, std::ios::beg);

        std::vector<char> file_bytes(file_size);
        input_stream.read(file_bytes.data(), file_size);
        return file_bytes;
    }

    std::vector<uint32_t> get_codepoints_from_utf8(const std::string &text) {
        std::vector<uint32_t> codepoints;
        size_t i = 0;
        while (i < text.size()) {
            // TODO Add continuation byte check

            uint32_t codepoint = 0;
            if ((text[i] & 0b10000000) == 0b00000000) {
                codepoint = text[i++];
            } else if ((text[i] & 0b11100000) == 0b11000000) {
                codepoint = ((text[i++] & 0x1F) << 6) | (text[i++] & 0x3F);
            } else if ((text[i] & 0b11110000) == 0b11100000) {
                codepoint = ((text[i++] & 0xF0) << 12) | ((text[i++] & 0x3F) << 6) |
                            (text[i++] & 0x3F);
            } else if ((text[i] & 0b11111000) == 0b11110000) {
                codepoint = ((text[i++] & 0x07) << 18) | ((text[i++] & 0x3F) << 12) |
                            ((text[i++] & 0x3F) << 6) | (text[i++] & 0x3F);
            } else {
                throw std::runtime_error("Invalid UTF-8 byte.");
            }

            codepoints.push_back(codepoint);
        }
        return codepoints;
    }

}
