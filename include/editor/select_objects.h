/**
 * @author Charlie Davidson 
 * Created on 7/9/22.
 */

#pragma once

#include "core/game_object.h"

namespace SelectObjects {

    void init();

    void reload();

    void update_selected_objects();

    void reset_highlighted();

    void reset_selected();

    void unselect_object(GameObject::game_object *obj);

    void highlight_objects();

    void select_object();

    void select_objects();

    void mouse_click();

    void shift_click();

    void mouse_released();

    void move_objects();

    void start_multiselect();

    void render_shadows();

    void copy_objects();

    void paste_objects();

    void delete_objects();

    void imgui();

}
