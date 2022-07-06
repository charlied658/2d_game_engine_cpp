/**
 * @author Charlie Davidson 
 * Created on 7/5/22.
 */

#pragma once

#include "core/game_object.h"

namespace Scene {

    void init();

    void update(double dt);

    void add_game_object(GameObject::game_object *obj);

}