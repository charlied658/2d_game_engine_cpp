/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#pragma once

namespace GameObject {
    struct game_object {
        const char *name;
        float x_pos, y_pos;
        float x_scale, y_scale;
        unsigned int textureID;
    };

    struct list {
        game_object game_objects[100];
        int count;
    };

    void init();

    void add_game_object(game_object *obj);

    GameObject::list *get_game_object_list();
}