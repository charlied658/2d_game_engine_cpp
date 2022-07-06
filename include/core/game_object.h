/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#pragma once

#include "core/sprite.h"

namespace GameObject {
    struct game_object {
        const char *name;
        float x_pos, y_pos;
        float x_scale, y_scale;
        int z_index;
        Sprite::sprite sprite;
        bool is_dirty;
        float r,g,b,a;
    };

    void init(GameObject::game_object *obj, const char *name, float x_pos, float y_pos, float x_scale, float y_scale, int z_index, Sprite::sprite *sprite);

    void set_sprite(GameObject::game_object *obj, Sprite::sprite *spr);

    void set_position(GameObject::game_object *obj, float xPos, float yPos);

    void set_color(GameObject::game_object *obj, float r, float g, float b, float a);

}