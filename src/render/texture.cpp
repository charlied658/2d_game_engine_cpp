/**
 * @author Charlie Davidson
 * Created on 7/2/22.
 */

#include "render/texture.h"

#include "lib/stb_image.h"
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

namespace Texture {

    /**
     * Generate a texture and return the texture ID.
     * @param filepath Filepath of texture
     * @return Texture ID
     */
    unsigned int create_texture(const char *filepath) {

        // Load image data
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(filepath, &width, &height, &channels, 0);

        if (!data) {
            printf("Failed to load texture\n");
            exit(EXIT_FAILURE);
        }

        // Generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        if (channels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else if (channels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else {
            printf("Error: Unknown number of channels: %d\n", channels);
            exit(EXIT_FAILURE);
        }

        // Free resources
        stbi_image_free(data);

        // Return textureID
        return texture;
    }
}
