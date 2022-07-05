/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#pragma once

#include "core/game_object.h"

namespace Render {

    void init();

    void render();

    static void add_game_object(GameObject::game_object *obj);

}
