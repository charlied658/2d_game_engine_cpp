/**
 * @author Charlie Davidson
 * Created on 7/2/22.
 */

#pragma once

#include <string>

namespace Texture {

    struct texture {
        unsigned int textureID;
        std::string filepath;
    };

    texture *get_texture(const std::string& filepath);

    static unsigned int create_texture(const std::string& filepath);
}
