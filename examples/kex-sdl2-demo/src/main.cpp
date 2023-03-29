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

#if KEX_USE_GLEW
#include <GL/glew.h>
#endif

int main() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        std::cout << "Could not initialize SDL: " << SDL_GetError() << '\n';
        std::exit(1);
    }

    SDL_Window *window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                                          SDL_WINDOW_OPENGL);

    SDL_GLContext ctx = SDL_GL_CreateContext(window);

    std::cout << "Hello from " << PROJECT_NAME << " v" << PROJECT_VERSION << '\n';
    kex::initialize();
    kex::Texture texture("/tmp/tex.png");
    kex::Sprite sprite(texture);
    kex::Sprite sprite2(texture);

    glViewport(0, 0, 800, 600);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    constexpr bool vsync = true;
    SDL_GL_SetSwapInterval(vsync);

    SDL_Event event;
    bool running = true;
    constexpr float amplitude = 100.f;
    constexpr float pi = 3.14f;
    float step = 0.f;
    Uint64 previous_ticks = SDL_GetTicks64();
    while (running) {
        const Uint64 ticks = SDL_GetTicks64();
        const float delta = (ticks - previous_ticks) / 1000.f;
        const float fps = 1 / delta;
        std::cout << "FPS: " << fps << '\n';
        previous_ticks = ticks;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        step += delta;
        sprite.set_position_x(SDL_sinf(step * pi * 2) * amplitude);
        sprite.set_position_y(SDL_cosf(step * pi * 2) * amplitude);

        sprite2.set_position_x(SDL_cosf(step * pi * 2 + pi) * amplitude);
        sprite2.set_position_y(SDL_sinf(step * pi * 2 + pi) * amplitude);

        glClear(GL_COLOR_BUFFER_BIT);
        {
            kex::SpriteBatch batch;
            kex::SpriteBatch batch2;
            batch.add(sprite);
            batch2.add(sprite2);
        }
        SDL_GL_SwapWindow(window);

    }

    kex::shutdown();
    SDL_Quit();
    return 0;
}
