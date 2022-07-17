/**
 * @author Charlie Davidson 
 * Created on 7/14/22.
 */

#pragma once

#include <glm/vec2.hpp> // glm::vec2

namespace BoxCollider {

    struct box_collider {
        glm::vec2 position;
        glm::vec2 scale;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(position, scale);
        }
    };

}
