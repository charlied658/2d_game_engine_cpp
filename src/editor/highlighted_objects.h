/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "core/sprite_manager.h"

namespace Highlight {

    void init();

    void reload();

    void update();

    static void reset();

    static void highlight_objects();

    void get_highlighted_object(SpriteManager::sprite_manager **object);

    void get_highlighted_objects(SpriteManager::sprite_manager ***objects, int *object_count);

    bool is_highlighted();

    bool is_selected();

}
