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
    };

    struct list {
        game_object game_objects[1000];
        int count;
    };

    void init();

    void add_game_object(game_object *obj);

    GameObject::list *get_game_object_list();
}