/**
 * @author Charlie Davidson 
 * Created on 7/14/22.
 */

#pragma once

#include "sprite_manager.h"

namespace SpriteSystem {

    void init();

    void reload();

    void add_sprite_manager(Editor::SpriteManager::sprite_manager *spr_manager);

    void remove_sprite_manager(Editor::SpriteManager::sprite_manager *spr_manager);

}
