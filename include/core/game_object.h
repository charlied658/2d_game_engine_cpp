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
        Sprite::sprite sprite;
        bool is_dirty;
    };

    void set_sprite(GameObject::game_object *obj, Sprite::sprite *spr);

    void set_position(GameObject::game_object *obj, float xPos, float yPos);

}