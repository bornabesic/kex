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

#ifndef KEX_KEX_HPP
#define KEX_KEX_HPP

namespace kex {

    using LoadProcedureFn = void *(*)(const char *name);

    extern int logical_viewport_w;
    extern int logical_viewport_h;

    void initialize(LoadProcedureFn load_fn);

    void set_viewport(int x, int y, int w, int h);

    void set_logical_viewport(int w, int h);

}

#endif //KEX_KEX_HPP
