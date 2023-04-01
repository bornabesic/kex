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

#include <memory>
#include <filesystem>

#ifdef KEX_USE_GLEW
#include <GL/glew.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <kex/texture.hpp>

namespace kex {

    class Texture::Impl {
    public:
        explicit Impl(const std::string &path, const bool mipmap) {
            if (!std::filesystem::exists(path)) {
                throw std::runtime_error(path + " does not exist.");
            }

            // Load the image
            int n_original_channels;
            stbi_set_flip_vertically_on_load(true);
            unsigned char *data = stbi_load(path.c_str(), &width, &height, &n_original_channels, 4);
            if (data == nullptr) {
                throw std::runtime_error("Could not load texture from " + path);
            }

            // Generate an OpenGL texture
            glGenTextures(1, &id);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            if (mipmap) {
                glGenerateMipmap(GL_TEXTURE_2D);
            }

            glBindTexture(GL_TEXTURE_2D, 0); // Unbind
            stbi_image_free(data); // Free image data from RAM
        }

        void bind() const {
            Texture::bind(id);
        }

        ~Impl() {
            glDeleteTextures(1, &id);
        }

    private:
        GLuint id = 0;
        int width = 0;
        int height = 0;

        friend Texture;
    };

    Texture::Texture(const std::string &path, const bool mipmap) : impl(
            std::make_unique<Texture::Impl>(path, mipmap)) {}

    void Texture::bind() const { impl->bind(); }

    int Texture::width() const { return impl->width; }

    int Texture::height() const { return impl->height; }

    unsigned int Texture::id() const { return impl->id; }

    void Texture::bind(unsigned int id) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    Texture::~Texture() = default;

} // kex
