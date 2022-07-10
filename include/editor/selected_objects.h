/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "core/game_object.h"

namespace Selected {

    void init();

    void reload();

    void start_frame();

    void reset_selected();

    void unselect_object(GameObject::game_object *obj);

    void select_object();

    void select_objects();

    void get_selected_objects(GameObject::game_object ***objects, int *object_count);

}
