/**
 * @author Charlie Davidson
 * Created on 7/2/22.
 */

#pragma once

#include <string>

using namespace std;

namespace Texture {

    struct texture {
        unsigned int textureID;
        string filepath;
    };

    texture *get_texture(const string& filepath);

    static unsigned int create_texture(const string& filepath);
}
