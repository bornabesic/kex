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

    std::vector<int> get_codepoints_from_utf8(const std::string &text) {
        std::vector<int> codepoints;
        for (const char byte : text) {
            // TODO

            if (byte & 0b10000000) {

            }
            else if ((byte & 0b11100000) == 0b11000000) {

            }
            else if ((byte & 0b11110000) == 0b11100000) {

            }
            else if ((byte & 0b11111000) == 0b11110000) {

            }
        }
        return codepoints;
    }

}
