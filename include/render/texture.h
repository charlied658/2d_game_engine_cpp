/**
 * @author Charlie Davidson
 * Created on 7/2/22.
 */

#pragma once

namespace Texture {

    struct texture {
        unsigned int textureID;
        const char* filepath;
    };

    texture *get_texture(const char *filepath);

    static unsigned int create_texture(const char *filepath);
}
