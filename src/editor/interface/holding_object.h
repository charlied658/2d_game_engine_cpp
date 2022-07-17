/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#pragma once

#include "core/sprite.h"
#include "editor/sprite_manager.h"

namespace Holding {

    void init();

    void generate_holding_object(Sprite::sprite *spr, const std::string& name);

    void update();

    void place_holding_object(bool destroy);

    void generate_game_object(Editor::GameObject::game_object *generated);

    void destroy_holding_object();

    void get_holding_object(Editor::GameObject::game_object **obj);

    bool is_holding();

    void set_holding(bool state);

}
