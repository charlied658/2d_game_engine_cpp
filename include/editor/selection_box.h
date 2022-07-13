/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "core/sprite.h"
#include "core/game_object.h"

namespace SelectionBox {

    void init();

    void reload();

    void start();

    void update();

    void hide();

    void get_selection_box(GameObject::game_object **object);

    bool is_multiselect();

}
