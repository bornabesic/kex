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

#include <iostream>
#include <cstdlib>

#define GLEW_NO_GLU
#include <GL/glew.h>

int main() {
    // TODO Move GLEW initialization into the library
    const GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        std::cout << "Could not initialize GLEW: " << glewGetErrorString(glew_status) << '\n';
        std::exit(1);
    }

    std::cout << "Hello from " << PROJECT_NAME << " v" << PROJECT_VERSION << '\n';
    return 0;
}
