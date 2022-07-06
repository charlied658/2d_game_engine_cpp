/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#include <cstdio>
#include "core/game_object.h"

namespace GameObject {

    /**
     * Initialize a game object
     * @param obj Game object reference
     * @param name Name
     * @param x_pos X position
     * @param y_pos Y position
     * @param x_scale X scale
     * @param y_scale Y scale
     * @param z_index Z-index
     * @param sprite Sprite reference
     */
    void init(GameObject::game_object *obj, const char *name, float x_pos, float y_pos, float x_scale, float y_scale, int z_index, Sprite::sprite *sprite) {
        obj->name = name;
        obj->x_pos = x_pos;
        obj->y_pos = y_pos;
        obj->x_scale = x_scale;
        obj->y_scale = y_scale;
        obj->z_index = z_index;
        obj->sprite = *sprite;
        obj->is_dirty = false;
        obj->r = 1.0f;
        obj->g = 1.0f;
        obj->b = 1.0f;
        obj->a = 1.0f;
    }

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

    /**
     * Set color of a game object
     * @param obj Game object reference
     * @param r Red value
     * @param g Green value
     * @param b Blue value
     * @param a Alpha value
     */
    void set_color(GameObject::game_object *obj, float r, float g, float b, float a) {
        obj->r = r;
        obj->g = g;
        obj->b = b;
        obj->a = a;
        obj->is_dirty = true;
    }
}
