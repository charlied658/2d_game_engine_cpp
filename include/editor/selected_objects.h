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

    void select_object();

    void unselect_object();

    void select_highlighted_object();

    void select_highlighted_objects();

    void select_holding_object(GameObject::game_object *obj);

    void get_selected_objects(GameObject::game_object ***objects, int *object_count);

    void set_selected_objects_count(int count);

}

