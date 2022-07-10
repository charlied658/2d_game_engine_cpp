/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#pragma once

#include <glm/vec2.hpp> // glm::vec2
#include <glm/vec4.hpp> // glm::vec4

#include <string>

#include "core/sprite.h"

namespace glm {
    template<class Archive> void serialize(Archive& archive, glm::vec4& v) { archive(v.x, v.y, v.z, v.w); }
}

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
        bool highlighted;
        bool selected;
        bool dragging;
        bool dead;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(name, position, scale, z_index, sprite, color, pickable);
        }
    };

    void init(GameObject::game_object *obj, string name, glm::vec2 position, glm::vec2 scale, int z_index, Sprite::sprite *sprite);

    void set_sprite(GameObject::game_object *obj, Sprite::sprite *spr);

    void set_position(GameObject::game_object *obj, glm::vec2 position);

    void set_scale(GameObject::game_object *obj, glm::vec2 scale);

    void set_color(GameObject::game_object *obj, glm::vec4 color);

    void set_visible(GameObject::game_object *obj, bool visible);

    void set_pickable(GameObject::game_object *obj, bool pickable);

    void set_highlighted(GameObject::game_object *obj, bool highlighted);

    void set_selected(GameObject::game_object *obj, bool selected);

    void set_dragging(GameObject::game_object *obj, bool dragging);

    void set_dead(GameObject::game_object *obj, bool dead);

    void update_color(GameObject::game_object *obj);

}