/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#pragma once

#include "glm/vec2.hpp" // glm::vec2
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4

#include <string>

#include "game_object.h"
#include "core/sprite.h"

namespace glm {
    template<class Archive> void serialize(Archive& archive, glm::vec4& v) { archive(v.x, v.y, v.z, v.w); }
}

namespace Editor {

    namespace GameObject {
        struct game_object;
    }

    namespace SpriteManager {
        struct sprite_manager {
            // Game object reference
            int id;
            Editor::GameObject::game_object *game_object;

            // Essential data
            glm::vec2 position;
            glm::vec2 scale;
            int z_index;
            Sprite::sprite sprite;
            glm::vec4 color;
            float saturation;

            // Editor information
            bool is_dirty;
            glm::vec4 out_color;
            bool pickable;
            bool visible;
            bool highlighted;
            bool selected;
            int new_z_index;
            int grid_x, grid_y;
            int last_grid_x, last_grid_y;
            glm::vec2 last_position;

            template<class Archive>
            void serialize(Archive &archive) {
                archive(id, position, scale, z_index, sprite, color, pickable, grid_x, grid_y);
            }
        };

        void init(Editor::SpriteManager::sprite_manager *obj, glm::vec2 position, glm::vec2 scale, int z_index, Sprite::sprite *sprite);

        void set_sprite(Editor::SpriteManager::sprite_manager *obj, Sprite::sprite *spr);

        void set_position(Editor::SpriteManager::sprite_manager *obj, glm::vec2 position);

        void set_scale(Editor::SpriteManager::sprite_manager *obj, glm::vec2 scale);

        void set_color(Editor::SpriteManager::sprite_manager *obj, glm::vec4 color);

        void set_saturation(Editor::SpriteManager::sprite_manager *obj, float saturation);

        void set_visible(Editor::SpriteManager::sprite_manager *obj, bool visible);

        void set_highlighted(Editor::SpriteManager::sprite_manager *obj, bool highlighted);

        void set_selected(Editor::SpriteManager::sprite_manager *obj, bool selected);

        void update_color(Editor::SpriteManager::sprite_manager *obj);

    }
}