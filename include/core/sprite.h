/**
 * @author Charlie Davidson 
 * Created on 7/5/22.
 */

#pragma once

#include <glm/vec2.hpp> // glm::vec2

namespace Sprite {
    struct sprite {
        unsigned int texture_ID;
        glm::vec2 tex_coords;
        glm::vec2 tex_scale;
    };
}
