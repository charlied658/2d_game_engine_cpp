/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#pragma once

#include <glm/vec2.hpp> // glm::vec2
#include <glm/vec4.hpp> // glm::vec4

#include <string>

#include "core/sprite.h"

using namespace std;

namespace GameObject {
    struct game_object {
        string name;
        glm::vec2 position;
        glm::vec2 scale;
        int z_index;
        Sprite::sprite sprite;
        bool is_dirty;
        glm::vec4 color;
        glm::vec4 out_color;
        bool pickable;
        bool visible;
        bool selected;
        bool dragging;
        bool active;
    };

    void init(GameObject::game_object *obj, string name, glm::vec2 position, glm::vec2 scale, int z_index, Sprite::sprite *sprite);

    void set_sprite(GameObject::game_object *obj, Sprite::sprite *spr);

    void set_position(GameObject::game_object *obj, glm::vec2 position);

    void set_scale(GameObject::game_object *obj, glm::vec2 scale);

    void set_color(GameObject::game_object *obj, glm::vec4 color);

    void set_selected(GameObject::game_object *obj, bool selected);

    void update_color(GameObject::game_object *obj);

}