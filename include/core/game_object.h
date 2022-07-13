/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#pragma once

#include <glm/vec2.hpp> // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

#include <string>

#include "core/sprite.h"

namespace glm {
    template<class Archive> void serialize(Archive& archive, glm::vec4& v) { archive(v.x, v.y, v.z, v.w); }
}

namespace GameObject {
    struct game_object {
        std::string name;
        glm::vec2 position;
        glm::vec2 scale;
        int z_index;
        Sprite::sprite sprite;
        bool is_dirty;
        glm::vec4 color;
        glm::vec4 out_color;
        float saturation;
        bool pickable;
        bool visible;
        bool highlighted;
        bool selected;
        bool dragging;
        bool dead;
        int new_z_index;
        int grid_x, grid_y;
        int last_grid_x, last_grid_y;
        glm::vec2 last_position;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(name, position, scale, z_index, sprite, color, pickable, grid_x, grid_y);
        }
    };

    void init(GameObject::game_object *obj, std::string name, glm::vec2 position, glm::vec2 scale, int z_index, Sprite::sprite *sprite);

    void set_sprite(GameObject::game_object *obj, Sprite::sprite *spr);

    void set_position(GameObject::game_object *obj, glm::vec2 position);

    void set_scale(GameObject::game_object *obj, glm::vec2 scale);

    void set_color(GameObject::game_object *obj, glm::vec4 color);

    void set_saturation(GameObject::game_object *obj, float saturation);

    void set_visible(GameObject::game_object *obj, bool visible);

    void set_highlighted(GameObject::game_object *obj, bool highlighted);

    void set_selected(GameObject::game_object *obj, bool selected);

    void set_dragging(GameObject::game_object *obj, bool dragging);

    void set_grid_position(GameObject::game_object *obj, int grid_x, int grid_y);

    void set_last_grid_position(GameObject::game_object *obj, int grid_x, int grid_y);

    void update_color(GameObject::game_object *obj);

}