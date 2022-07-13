/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#pragma once

#include "glm/vec2.hpp" // glm::vec2
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4

#include <string>

#include "sprite.h"

namespace glm {
    template<class Archive> void serialize(Archive& archive, glm::vec4& v) { archive(v.x, v.y, v.z, v.w); }
}

namespace SpriteManager {
    struct sprite_manager {
        std::string name;
        glm::vec2 position;
        glm::vec2 scale;
        int z_index;
        Sprite::sprite sprite;
        bool is_dirty;
        glm::vec4 color;
        glm::vec4 out_color;
        float saturation;
        bool pickable;
        bool visible;
        bool highlighted;
        bool selected;
        bool dragging;
        bool dead;
        int new_z_index;
        int grid_x, grid_y;
        int last_grid_x, last_grid_y;
        glm::vec2 last_position;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(name, position, scale, z_index, sprite, color, pickable, grid_x, grid_y);
        }
    };

    void init(SpriteManager::sprite_manager *obj, std::string name, glm::vec2 position, glm::vec2 scale, int z_index, Sprite::sprite *sprite);

    void set_sprite(SpriteManager::sprite_manager *obj, Sprite::sprite *spr);

    void set_position(SpriteManager::sprite_manager *obj, glm::vec2 position);

    void set_scale(SpriteManager::sprite_manager *obj, glm::vec2 scale);

    void set_color(SpriteManager::sprite_manager *obj, glm::vec4 color);

    void set_saturation(SpriteManager::sprite_manager *obj, float saturation);

    void set_visible(SpriteManager::sprite_manager *obj, bool visible);

    void set_highlighted(SpriteManager::sprite_manager *obj, bool highlighted);

    void set_selected(SpriteManager::sprite_manager *obj, bool selected);

    void set_dragging(SpriteManager::sprite_manager *obj, bool dragging);

    void update_color(SpriteManager::sprite_manager *obj);

}