/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "core/sprite.h"
#include "core/spritesheet.h"

namespace ObjectPicker {

    void init();

    void reset();

    void imgui();

    void dynamic_button_grid(Sprite::sprite **sprite_list, int sprite_count, const std::string& list_name, int *selected_block);

    void set_current_list(Sprite::sprite *list, int *count);

    void set_current_spritesheet(Spritesheet::spritesheet *spr_sheet);

    void add_sprite(int spr_index);

}
