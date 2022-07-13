/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#pragma once

#include <glm/vec2.hpp> // glm::vec2

#include "core/sprite_manager.h"


namespace Preview {

    void init();

    void reload();

    void update();

    void render();

    void render_holding();

    void hide();

    void generate_preview_object(SpriteManager::sprite_manager *obj, glm::vec2 position, glm::vec2 scale, Sprite::sprite *sprite);

}
