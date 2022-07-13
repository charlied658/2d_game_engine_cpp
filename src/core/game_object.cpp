/**
 * @author Charlie Davidson
 * Created on 7/3/22.
 */

#include <utility>
#include "core/game_object.h"

#include "editor/mouse_events.h"

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
        obj->new_z_index = z_index;
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
     * Set the sprite for a game object.
     * @param obj Game object
     * @param spr Sprite
     */
    void set_sprite(GameObject::game_object *obj, Sprite::sprite *spr) {
        obj->sprite = *spr;
        obj->is_dirty = true;
    }

    /**
     * Set position of a game object.
     * @param obj Game object
     * @param position Position
     */
    void set_position(GameObject::game_object *obj, glm::vec2 position) {
        obj->position = position;
        obj->is_dirty = true;
    }

    /**
     * Set scale of a game object.
     * @param obj Game object
     * @param scale Scale
     */
    void set_scale(GameObject::game_object *obj, glm::vec2 scale) {
        obj->scale = scale;
        obj->is_dirty = true;
    }

    /**
     * Set color of a game object.
     * @param obj Game object
     * @param color Color
     */
    void set_color(GameObject::game_object *obj, glm::vec4 color) {
        obj->color = color;
        obj->out_color = color;
        obj->is_dirty = true;
        update_color(obj);
    }

    /**
     * Set the saturation of a game object.
     * @param obj Game object
     * @param saturation Saturation
     */
    void set_saturation(GameObject::game_object *obj, float saturation) {
        obj->saturation = saturation;
        obj->is_dirty = true;
    }

    /**
     * Set a game object to be visible.
     * @param obj Game object
     * @param visible Visible
     */
    void set_visible(GameObject::game_object *obj, bool visible) {
        obj->visible = visible;
        update_color(obj);
    }

    /**
     * Set a game object to be highlighted.
     * @param obj Game object
     * @param highlighted Highlighted
     */
    void set_highlighted(GameObject::game_object *obj, bool highlighted) {
        obj->highlighted = highlighted;
        update_color(obj);
    }

    /**
     * Set a game object to be selected.
     * @param obj Game object
     * @param selected Selected
     */
    void set_selected(GameObject::game_object *obj, bool selected) {
        obj->selected = selected;
        update_color(obj);
    }

    /**
     * Set a game object to be dragging.
     * @param obj Game object
     * @param dragging Dragging
     */
    void set_dragging(GameObject::game_object *obj, bool dragging) {
        obj->dragging = dragging;
        update_color(obj);
    }

    /**
     * Update the output color of a game object
     * @param obj Game object
     */
    void update_color(GameObject::game_object *obj) {
        glm::vec3 color_offset {};
        obj->saturation = 0.2f;
        if (obj->selected) {
            if (Mouse::is_invalid_placement()) {
                obj->saturation = 0.4f;
                color_offset = glm::vec3{1.0f, 0.3f, 0.3f};
            } else {
                if (obj->highlighted) {
                    color_offset = glm::vec3{0.3f, 0.3f, 1.0f};
                } else {
                    color_offset = glm::vec3{0.0f, 0.0f, 1.0f};
                }
            }
            obj->out_color.x = color_offset.x;
            obj->out_color.y = color_offset.y;
            obj->out_color.z = color_offset.z;
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
