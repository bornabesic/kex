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

#ifndef KEX_KEX_H
#define KEX_KEX_H

namespace kex {

    // TODO Remove this
    struct SpriteBuffers {
        unsigned int v_positions = 0;
        unsigned int v_tex_coords = 0;
        unsigned int s_positions = 0;
        unsigned int s_sizes = 0;
    };

    extern SpriteBuffers sprite_buffers;

    void initialize();
    void shutdown();
}

#endif //KEX_KEX_H
