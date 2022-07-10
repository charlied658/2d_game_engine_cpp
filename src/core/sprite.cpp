/**
 * @author Charlie Davidson 
 * Created on 7/5/22.
 */

#include "core/sprite.h"

namespace Sprite {

    /**
     * Create a null sprite (Renders as solid color).
     * @param spr Sprite reference
     */
    void get_null_sprite(Sprite::sprite *spr) {
        Sprite::sprite sprite_null = Sprite::sprite {0, glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}};
        sprite_null.is_null = true;
        *spr = sprite_null;
    }
}
