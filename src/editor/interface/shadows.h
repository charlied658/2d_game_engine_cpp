/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "editor/sprite_manager.h"

namespace Shadows {

    void init();

    void render();

    void render_holding();

    void generate_shadow(Editor::SpriteManager::sprite_manager *obj, glm::vec2 position, glm::vec2 scale, Sprite::sprite *sprite);

}
