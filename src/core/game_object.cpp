/**
 * @author Charlie Davidson
 * Created on 7/3/22.
 */

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
    void init(GameObject::game_object *obj, std::string name, glm::vec2 position, glm::vec2 scale, int z_index, Sprite::sprite *sprite) {
        obj->name = std::move(name);
        obj->position = position;
        obj->scale = scale;
        obj->z_index = z_index;
        obj->sprite = *sprite;
        obj->is_dirty = true;
        obj->color = glm::vec4 {1.0f, 1.0f, 1.0f, 1.0f};
        obj->out_color = glm::vec4 {1.0f, 1.0f, 1.0f, 1.0f};
        obj->saturation = 0.0f;
        obj->pickable = true;
        obj->visible = true;
        obj->highlighted = false;
        obj->dragging = false;
        obj->selected = false;
        obj->dead = false;
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
     * @param color Color of object
     */
    void set_color(GameObject::game_object *obj, glm::vec4 color) {
        obj->color = color;
        obj->out_color = color;
        obj->is_dirty = true;
        update_color(obj);
    }

    /**
     * Set the saturation of a game object
     * @param saturation
     */
    void set_saturation(GameObject::game_object *obj, float saturation) {
        obj->saturation = saturation;
        obj->is_dirty = true;
    }

    /**
     * Set a game object to be visible or not.
     * @param obj Game object reference
     * @param visible Set whether the object is visible
     */
    void set_visible(GameObject::game_object *obj, bool visible) {
        if (obj->visible == visible) {
            return;
        }
        obj->visible = visible;
        update_color(obj);
    }

    /**
     * Set a game object to be pickable or not.
     * @param obj Game object reference
     * @param pickable Set whether the object is pickable
     */
    void set_pickable(GameObject::game_object *obj, bool pickable) {
        if (obj->pickable == pickable) {
            return;
        }
        obj->pickable = pickable;
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
     * Set a game object to be dead. It will be deleted shortly.
     * @param obj Game object reference
     * @param dead Set the object to be dead
     */
    void set_dead(GameObject::game_object *obj, bool dead) {
        if (obj->dead == dead) {
            return;
        }
        obj->dead = dead;
    }

    /**
     * Update the output color of a game object
     * @param obj Game object reference
     */
    void update_color(GameObject::game_object *obj) {
        glm::vec3 color_offset {};
        obj->saturation = 0.2f;
        if (obj->dragging) {
            color_offset = glm::vec3 {-0.1f,-0.1f,-0.1f};
        } else if (obj->selected) {
            if (obj->highlighted) {
                color_offset = glm::vec3 {0.3f,0.3f,1.0f};
                obj->out_color.x = color_offset.x;
                obj->out_color.y = color_offset.y;
                obj->out_color.z = color_offset.z;
            } else {
                color_offset = glm::vec3 {0.0f,0.0f,1.0f};
                obj->out_color.x = color_offset.x;
                obj->out_color.y = color_offset.y;
                obj->out_color.z = color_offset.z;
            }
        } else {
            if (obj->highlighted) {
                obj->out_color.x = obj->color.x + 0.2f;
                obj->out_color.y = obj->color.y + 0.2f;
                obj->out_color.z = obj->color.z + 0.2f;
            } else {
                obj->saturation = 0.0f;
                obj->out_color.x = obj->color.x;
                obj->out_color.y = obj->color.y;
                obj->out_color.z = obj->color.z;
            }
        }

        if (obj->visible) {
            obj->out_color.w = obj->color.w;
        } else {
            obj->saturation = 1.0f;
            obj->out_color.w = 0.0f;
        }
        obj->is_dirty = true;
    }
}
