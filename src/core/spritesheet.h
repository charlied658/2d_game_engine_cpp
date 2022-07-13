/**
 * @author Charlie Davidson 
 * Created on 7/5/22.
 */

#pragma once

#include "sprite.h"

namespace Spritesheet {
    struct spritesheet {
        int width, height;
        unsigned int texture_ID;
        std::string filepath_texture;
    };

    void init(Spritesheet::spritesheet *spr_sheet, int width, int height, const std::string& filepath);

    Sprite::sprite get_sprite(Spritesheet::spritesheet *spr_sheet, int index);
}
