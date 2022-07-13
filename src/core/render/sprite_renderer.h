/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#pragma once

#include "core/game_object.h"

namespace SpriteRenderer {

    void render();

    void add_game_object(GameObject::game_object *obj);

    void remove_game_object(GameObject::game_object *obj);

    void re_add_game_object(GameObject::game_object *obj);

    void highlight_game_object(GameObject::game_object **highlighted_obj);

    void highlight_game_objects(GameObject::game_object **highlighted_objects, int *highlighted_count, GameObject::game_object *selection_box);

    void clear_render_batches();

}
