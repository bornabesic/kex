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
#include <iostream>

#include <kex/kex.hpp>
#include <kex/def.hpp>

#ifdef KEX_USE_GLEW
#include <GL/glew.h>
#endif

namespace kex {

    int logical_viewport_w;
    int logical_viewport_h;

    void initialize() {
#ifdef KEX_USE_GLEW
        glewExperimental = GL_TRUE;
        const GLenum glew_status = glewInit();
        if (glew_status != GLEW_OK) {
            throw std::runtime_error(reinterpret_cast<const char *>(glewGetErrorString(glew_status)));
        }
#endif

        RectangleDef opengl_viewport{};
        glGetIntegerv(GL_VIEWPORT, reinterpret_cast<GLint *>(&opengl_viewport));
        logical_viewport_w = opengl_viewport.w;
        logical_viewport_h = opengl_viewport.h;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';
    }

    void set_viewport(int x, int y, int w, int h) {
        glViewport(x, y, w, h);
    }

    void set_logical_viewport(int w, int h) {
        logical_viewport_w = w;
        logical_viewport_h = h;
    }

}
