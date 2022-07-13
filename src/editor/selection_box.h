/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "core/sprite.h"
#include "core/sprite_manager.h"

namespace SelectionBox {

    void init();

    void reload();

    void start();

    void update();

    void hide();

    void get_selection_box(SpriteManager::sprite_manager **object);

    bool is_multiselect();

}
