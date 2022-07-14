/**
 * @author Charlie Davidson
 * Created on 7/5/22.
 */

#include "spritesheet.h"
#include "texture.h"

namespace Spritesheet {

    /**
     * Initialize a spritesheet.
     * @param spr_sheet Spritesheet reference
     * @param width Number of sprites along x-axis
     * @param height Number of sprites along y-axis
     * @param filepath Filepath of texture
     */
    void init(Spritesheet::spritesheet *spr_sheet, int width, int height, const std::string& filepath) {
        spr_sheet->width = width;
        spr_sheet->height = height;
        spr_sheet->texture_ID = Texture::get_texture(filepath)->textureID;
        spr_sheet->filepath_texture = filepath;
    }

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
        spr.tex_coords.y = (float) (spr_sheet->height - (index / spr_sheet->width) - 1) / (float) spr_sheet->height;
        spr.tex_scale.x = 1.0f / (float) spr_sheet->width;
        spr.tex_scale.y = 1.0f / (float) spr_sheet->height;
        spr.texture_filepath = spr_sheet->filepath_texture;
        spr.is_null = false;
        return spr;
    }

}
