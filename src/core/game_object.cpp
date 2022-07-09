/**
 * @author Charlie Davidson
 * Created on 7/3/22.
 */

#include <cstdio>
#include <utility>
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
    void init(GameObject::game_object *obj, string name, glm::vec2 position, glm::vec2 scale, int z_index, Sprite::sprite *sprite) {
        obj->name = std::move(name);
        obj->position = position;
        obj->scale = scale;
        obj->z_index = z_index;
        obj->sprite = *sprite;
        obj->is_dirty = true;
        obj->color = glm::vec4 {1.0f, 1.0f, 1.0f, 1.0f};
        obj->out_color = glm::vec4 {1.0f, 1.0f, 1.0f, 1.0f};
        obj->pickable = true;
        obj->visible = true;
        obj->highlighted = false;
        obj->dragging = false;
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
     * Set scale of a game object
     * @param obj Game object reference
     * @param scale Scale
     */
    void set_scale(GameObject::game_object *obj, glm::vec2 scale) {
        obj->scale = scale;
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
        obj->out_color = color;
        obj->is_dirty = true;
        update_color(obj);
    }

    /**
     * Set a game object to be highlighted or not.
     * @param obj Game object reference
     * @param highlighted Set whether the object is highlighted
     */
    void set_highlighted(GameObject::game_object *obj, bool highlighted) {
        if (obj->highlighted == highlighted) {
            return;
        }
        obj->highlighted = highlighted;
        update_color(obj);
    }

    /**
     * Set a game object to be selected or not.
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
     * Set a game object to be dragging or not.
     * @param obj Game object reference
     * @param dragging Set whether the object is dragging
     */
    void set_dragging(GameObject::game_object *obj, bool dragging) {
        if (obj->dragging == dragging) {
            return;
        }
        obj->dragging = dragging;
        update_color(obj);
    }

    /**
     * Update the output color of a game object
     * @param obj Game object reference
     */
    void update_color(GameObject::game_object *obj) {

        if (obj->dragging) {
            obj->out_color.x = obj->color.x - 0.1f;
            obj->out_color.y = obj->color.y - 0.1f;
            obj->out_color.z = obj->color.z - 0.1f;
        } else if (obj->highlighted) {
            obj->out_color.x = obj->color.x + 0.2f;
            obj->out_color.y = obj->color.y + 0.2f;
            obj->out_color.z = obj->color.z + 0.2f;
        } else {
            obj->out_color.x = obj->color.x;
            obj->out_color.y = obj->color.y;
            obj->out_color.z = obj->color.z;
        }
        if (obj->visible) {
            obj->out_color.w = obj->color.w;
        } else {
            obj->out_color.w = 0.0f;
        }
        obj->is_dirty = true;
    }
}
