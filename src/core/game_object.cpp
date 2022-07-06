/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#include <cstdio>
#include "core/game_object.h"

namespace GameObject {

    /**
     * Set the sprite for a game object
     * @param obj Game object reference
     * @param spr Sprite
     */
    void set_sprite(GameObject::game_object *obj, Sprite::sprite *spr) {
        obj->sprite = *spr;
        obj->is_dirty = true;
    }

    /**
     * Set position of a game object
     * @param obj Game object reference
     * @param xPos X position
     * @param yPos Y position
     */
    void set_position(GameObject::game_object *obj, float xPos, float yPos) {
        obj->x_pos = xPos;
        obj->y_pos = yPos;
        obj->is_dirty = true;
    }
}
