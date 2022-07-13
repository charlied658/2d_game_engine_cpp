/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "core/game_object.h"

namespace Shadows {

    void init();

    void reload();

    void render();

    void render_holding();

    void generate_shadow(GameObject::game_object *obj, glm::vec2 position, glm::vec2 scale, Sprite::sprite *sprite);

}
