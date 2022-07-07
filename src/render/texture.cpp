/**
 * @author Charlie Davidson
 * Created on 7/2/22.
 */

#include "render/texture.h"
#include "util/properties.h"

#include <stb/stb_image.h>
#include <glad/glad.h>

#include <string>

using namespace std;

namespace Texture {

    static texture texture_list[100];
    static int texture_count;

    /**
     * Get a texture from the texture list. If it is not in the list, add it to the list.
     * @param filepath Filepath of texture
     * @return Texture
     */
    texture *get_texture(const string& filepath) {
        string absolute_filepath = PROJECT_PATH + filepath;
        for (int i = 0; i < texture_count; i++) {
            if (texture_list[i].filepath == absolute_filepath) {
                return &texture_list[i];
            }
        }
        // If no match is found, create a texture and add it to the texture list.
        texture_list[texture_count] = texture {};
        texture_list[texture_count].filepath = filepath;
        texture_list[texture_count].textureID = create_texture(absolute_filepath);
        printf("Created texture '%s'\n", absolute_filepath.c_str());
        texture *to_return = &texture_list[texture_count];
        texture_count++;
        return to_return;
    }

    /**
     * Generate a texture and return the texture ID.
     * @param filepath Filepath of texture
     * @return Texture ID
     */
    static unsigned int create_texture(const string& filepath) {

        // Load image data
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        if (!data) {
            printf("Failed to load texture '%s'\n", filepath.c_str());
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
