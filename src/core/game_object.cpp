/**
 * @author Charlie Davidson 
 * Created on 7/3/22.
 */

#include <cstdio>
#include "core/game_object.h"

namespace GameObject {

    static GameObject::list game_object_list;

    void init() {
        game_object_list.count = 0;
    }

    void add_game_object(game_object *obj) {
        if (game_object_list.count < 100) {
            game_object_list.game_objects[game_object_list.count] = *obj;
            game_object_list.count++;
        }
    }

    GameObject::list *get_game_object_list() {
        return &game_object_list;
    }

}
