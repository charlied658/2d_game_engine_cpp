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
     * @param position Position
     * @param scale Scale
     * @param z_index Z-index
     * @param sprite Sprite reference
     */
    void init(GameObject::game_object *obj, const char *name, glm::vec2 position, glm::vec2 scale, int z_index, Sprite::sprite *sprite) {
        obj->name = name;
        obj->position = position;
        obj->scale = scale;
        obj->z_index = z_index;
        obj->sprite = *sprite;
        obj->is_dirty = false;
        obj->color = glm::vec4 {1.0f, 1.0f, 1.0f, 1.0f};
        obj->out_color = glm::vec4 {1.0f, 1.0f, 1.0f, 1.0f};
        obj->selected = false;
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
     * @param position Position
     */
    void set_position(GameObject::game_object *obj, glm::vec2 position) {
        obj->position = position;
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
    void set_color(GameObject::game_object *obj, glm::vec4 color) {
        obj->color = color;
        update_color(obj);
    }

    /**
     * Set a game object to be selected or not
     * @param obj Game object reference
     * @param selected Set whether the object is selected
     */
    void set_selected(GameObject::game_object *obj, bool selected) {
        if (obj->selected == selected) {
            return;
        }
        obj->selected = selected;
        update_color(obj);
    }

    /**
     * Update the output color of a game object
     * @param obj Game object reference
     */
    void update_color(GameObject::game_object *obj) {
        obj->out_color.x = obj->color.x;
        obj->out_color.y = obj->color.y;
        obj->out_color.z = obj->color.z;
        if (obj->selected) {
            obj->out_color.w = obj->color.w * 0.8f;
        } else {
            obj->out_color.w = obj->color.w;
        }
        obj->is_dirty = true;
    }
}
