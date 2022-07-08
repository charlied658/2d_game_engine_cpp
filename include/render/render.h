/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#pragma once

#include "core/game_object.h"

namespace Render {

    void render();

    void add_game_object(GameObject::game_object *obj);

    void select_game_object(GameObject::game_object **selected_obj);

    void select_game_objects(GameObject::game_object **selected_objects, int *selected_count, glm::vec2 selection_pos, glm::vec2 selection_scale);

}
