/**
 * @author Charlie Davidson
 * Created on 7/3/22.
 */

#include <utility>
#include "sprite_manager.h"

namespace Game {
    namespace SpriteManager {

        void
        init(Game::SpriteManager::sprite_manager *obj, std::string name, glm::vec2 position, glm::vec2 scale, int z_index,
             Sprite::sprite *sprite) {
            obj->name = std::move(name);
            obj->position = position;
            obj->scale = scale;
            obj->z_index = z_index;
            obj->sprite = *sprite;
            obj->color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
            obj->saturation = 0.0f;
            obj->is_dirty = true;
            obj->new_z_index = z_index;
            obj->out_color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
            obj->dead = false;
        }

        void set_sprite(Game::SpriteManager::sprite_manager *obj, Sprite::sprite *spr) {
            obj->sprite = *spr;
            obj->is_dirty = true;
        }

        void set_position(Game::SpriteManager::sprite_manager *obj, glm::vec2 position) {
            obj->position = position;
            obj->is_dirty = true;
        }

        void set_scale(Game::SpriteManager::sprite_manager *obj, glm::vec2 scale) {
            obj->scale = scale;
            obj->is_dirty = true;
        }

        void set_color(Game::SpriteManager::sprite_manager *obj, glm::vec4 color) {
            obj->color = color;
            obj->out_color = color;
            obj->is_dirty = true;
            update_color(obj);
        }

        void set_saturation(Game::SpriteManager::sprite_manager *obj, float saturation) {
            obj->saturation = saturation;
            obj->is_dirty = true;
        }

        /**
         * Update the output color of a sprite
         * @param obj Game object
         */
        void update_color(Game::SpriteManager::sprite_manager *obj) {
            obj->out_color.x = obj->color.x;
            obj->out_color.y = obj->color.y;
            obj->out_color.z = obj->color.z;
            obj->out_color.w = obj->color.w;
            obj->is_dirty = true;
        }
    }
}
