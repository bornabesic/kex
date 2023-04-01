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

    /**
     * Width of the logical viewport.
     */
    extern int logical_viewport_w;

    /**
     * Height of the logical viewport.
     */
    extern int logical_viewport_h;

    /**
     * Initialize the Kex library.
     *
     * The library is currently initialized the following way:
     *     -# OpenGL ES 3.0 procedures are loaded via @p load_fn
     *     -# The logical viewport is initialized with the current dimensions of the OpenGL viewport
     *     -# Blending is enabled
     *
     * @verbatim embed:rst:leading-asterisk
     * .. note::
     *    Kex does not manage OpenGL contexts. A user must ensure a valid context is available
     *    before initializing the library.
     * @endverbatim
     *
     * @param load_fn Function that loads a `void *` pointer to an OpenGL procedure specified by `const char *`
     */
    void initialize(LoadProcedureFn load_fn);

    /**
     * Set the OpenGL viewport.
     *
     * Wrapper for
     * [glViewport](https://registry.khronos.org/OpenGL-Refpages/es3.0/html/glViewport.xhtml).
     *
     * @param x Bottom left x-coordinate of the viewport (px)
     * @param y Bottom left y-coordinate of the viewport (px)
     * @param w Viewport width (px)
     * @param h Viewport height (px)
     */
    void set_viewport(int x, int y, int w, int h);

    /**
     * Set the size of the logical viewport.
     *
     * This utility function is equal to
     * @code{.cpp}
     * kex::logical_viewport_w = w;
     * kex::logical_viewport_h = h;
     * @endcode
     *
     * @verbatim embed:rst:leading-asterisk
     * .. note::
     *    Logical viewport allows rendering that is independent of the output resolution.
     * @endverbatim
     *
     * @param w Width of the logical viewport (px)
     * @param h Height of the logical viewport (px)
     */
    void set_logical_viewport(int w, int h);

}

#endif //KEX_KEX_HPP
