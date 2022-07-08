/**
 * @author Charlie Davidson
 * Created on 7/5/22.
 */

#include "core/spritesheet.h"

namespace Spritesheet {

    /**
     * Get a sprite from the given spritesheet.
     * @param spr_sheet Spritesheet reference
     * @param index Index of desired sprite
     * @return Sprite struct with valid texture ID and coordinates
     */
    Sprite::sprite get_sprite(Spritesheet::spritesheet *spr_sheet, int index) {
        Sprite::sprite spr {};
        spr.texture_ID = spr_sheet->texture_ID;
        spr.tex_coords.x = (float) (index % spr_sheet->width) / (float) spr_sheet->width;
        spr.tex_coords.y = (float) ((index / spr_sheet->width) + 1) / (float) spr_sheet->height;
        spr.tex_scale.x = 1.0f / (float) spr_sheet->width;
        spr.tex_scale.y = 1.0f / (float) spr_sheet->height;
        spr.is_null = false;
        return spr;
    }

}
