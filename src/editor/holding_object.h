/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#pragma once

#include "core/sprite.h"
#include "core/sprite_manager.h"

namespace Holding {

    void init();

    void reload();

    void generateHoldingObject(Sprite::sprite *spr, const std::string& name);

    void update();

    void place_holding_object(bool destroy);

    void destroy_holding_object();

    void get_holding_object(SpriteManager::sprite_manager **obj);

    bool is_holding();

    void set_holding(bool state);

}
