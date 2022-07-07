/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#pragma once

#include <glm/glm.hpp>

#include "core/sprite.h"

namespace GameObject {
    struct game_object {
        const char *name;
        glm::vec2 position;
        glm::vec2 scale;
        int z_index;
        Sprite::sprite sprite;
        bool is_dirty;
        glm::vec4 color;
        glm::vec4 out_color;
        bool selected;
    };

    void init(GameObject::game_object *obj, const char *name, glm::vec2 position, glm::vec2 scale, int z_index, Sprite::sprite *sprite);

    void set_sprite(GameObject::game_object *obj, Sprite::sprite *spr);

    void set_position(GameObject::game_object *obj, glm::vec2 position);

    void set_color(GameObject::game_object *obj, glm::vec4 color);

    void set_selected(GameObject::game_object *obj, bool selected);

    void update_color(GameObject::game_object *obj);

}