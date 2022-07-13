/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "core/sprite_manager.h"

namespace Copy {

    void init();

    void reload();

    void start_frame();

    void copy_objects();

    void paste_objects();

    void delete_objects();

    void get_copied_objects(SpriteManager::sprite_manager ***objects, int *object_count);

    bool get_copied();

    void set_copied(bool state);

}
