/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "core/sprite.h"

namespace ObjectPicker {

    void init();

    void imgui();

    void dynamic_button_grid(Sprite::sprite **sprite_list, int sprite_count);

}
