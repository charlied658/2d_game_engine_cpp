/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#pragma once

#include <glm/vec2.hpp> // glm::vec2

#include "core/game_object.h"


namespace Preview {

    void init();

    void reload();

    void update();

    void render();

    void render_holding();

    void hide();

    void generate_preview_object(GameObject::game_object *obj, glm::vec2 position, glm::vec2 scale, Sprite::sprite *sprite);

}
