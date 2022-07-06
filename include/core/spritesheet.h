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
    };

    Sprite::sprite get_sprite(Spritesheet::spritesheet *spr_sheet, int index);
}
