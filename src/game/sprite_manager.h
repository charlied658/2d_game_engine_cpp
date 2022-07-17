/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#pragma once

#include "glm/vec2.hpp" // glm::vec2
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4

#include <string>

#include "core/sprite.h"

namespace Game {
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
            bool dead;
            int new_z_index;
            int grid_x, grid_y;

            template<class Archive>
            void serialize(Archive &archive) {
                archive(name, position, scale, z_index, sprite, color, grid_x, grid_y);
            }
        };

        void
        init(Game::SpriteManager::sprite_manager *obj, std::string name, glm::vec2 position, glm::vec2 scale, int z_index,
             Sprite::sprite *sprite);

        void set_sprite(Game::SpriteManager::sprite_manager *obj, Sprite::sprite *spr);

        void set_position(Game::SpriteManager::sprite_manager *obj, glm::vec2 position);

        void set_scale(Game::SpriteManager::sprite_manager *obj, glm::vec2 scale);

        void set_color(Game::SpriteManager::sprite_manager *obj, glm::vec4 color);

        void set_saturation(Game::SpriteManager::sprite_manager *obj, float saturation);

        void update_color(Game::SpriteManager::sprite_manager *obj);

    }
}