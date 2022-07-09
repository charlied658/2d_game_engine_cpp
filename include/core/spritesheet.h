/**
 * @author Charlie Davidson 
 * Created on 7/5/22.
 */

#pragma once

#include "core/sprite.h"

namespace Spritesheet {
    struct spritesheet {
        int width, height;
        unsigned int texture_ID;
        string filepath_texture;
    };

    void init(Spritesheet::spritesheet *spr_sheet, int width, int height, const string& filepath);

    Sprite::sprite get_sprite(Spritesheet::spritesheet *spr_sheet, int index);
}
