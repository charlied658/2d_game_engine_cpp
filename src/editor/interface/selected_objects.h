/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "editor/sprite_manager.h"

namespace Selected {

    void init();

    void reload();

    void start_frame();

    void reset_selected();

    void select_object();

    void unselect_object();

    void select_highlighted_object();

    void select_highlighted_objects();

    void select_holding_object(Editor::SpriteManager::sprite_manager *obj);

    void get_selected_objects(Editor::SpriteManager::sprite_manager ***objects, int *object_count);

    void set_selected_objects_count(int count);

}

