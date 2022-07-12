/**
 * @author Charlie Davidson 
 * Created on 7/5/22.
 */

#pragma once

#include <glm/vec2.hpp> // glm::vec2

#include <string>

namespace glm {
    template<class Archive> void serialize(Archive& archive, glm::vec2& v) { archive(v.x, v.y); }
}

namespace Sprite {
    struct sprite {
        unsigned int texture_ID;
        glm::vec2 tex_coords;
        glm::vec2 tex_scale;
        std::string texture_filepath;
        bool is_null;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive( texture_ID, texture_filepath, tex_coords, tex_scale, is_null);
        }
    };

    void get_null_sprite(Sprite::sprite *spr);
}
