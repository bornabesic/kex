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

#include <SDL.h>

#include <kex/kex.h>
#include <kex/texture.h>
#include <kex/sprite.h>
#include <kex/spritebatch.h>
#include <kex/shader.h>

int main() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        std::cout << "Could not initialize SDL: " << SDL_GetError() << '\n';
        std::exit(1);
    }

    SDL_Window *window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);

    SDL_GLContext ctx = SDL_GL_CreateContext(window);

    kex::initialize();
    kex::Texture texture("/tmp/tex.png");
    kex::Sprite sprite(texture);
    kex::VertexShader vertex_shader("void main() { gl_Position = vec4(0, 0, 0, 1); }");
    {
        kex::SpriteBatch batch;
        batch.add(sprite);
    }

    std::cout << "Hello from " << PROJECT_NAME << " v" << PROJECT_VERSION << '\n';
    return 0;
}
