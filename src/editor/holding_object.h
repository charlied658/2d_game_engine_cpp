/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#pragma once

#include "core/sprite.h"
#include "core/game_object.h"

namespace Holding {

    void init();

    void reload();

    void generateHoldingObject(Sprite::sprite *spr, const std::string& name);

    void update();

    void place_holding_object(bool destroy);

    void destroy_holding_object();

    void get_holding_object(GameObject::game_object **obj);

    bool is_holding();

    void set_holding(bool state);

}
