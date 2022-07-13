/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/selection_box.h"

#include "core/render/sprite_renderer.h"
#include "editor/mouse_events.h"

namespace SelectionBox {

    static Sprite::sprite sprite_null;
    static SpriteManager::sprite_manager selection_box;
    static double start_x, start_y;
    static bool multiselect;

    /**
     * Initialize selection box.
     */
    void init() {
        // Generate null sprite (Renders as a solid color)
        Sprite::get_null_sprite(&sprite_null);

        // Generate selection box (Appears when you click and drag to multiselect)
        SpriteManager::init(&selection_box, "selection_box", glm::vec2 {1.0f, 1.5f}, glm::vec2 {1.0f, 1.0f}, 2, &sprite_null);
        SpriteManager::set_color(&selection_box, glm::vec4 {1.0f, 1.0f, 0.0f, 0.2f});
        selection_box.pickable = false;
        SpriteManager::set_visible(&selection_box, false);
        SpriteRenderer::add_sprite(&selection_box);
    }

    /**
     * Reset selection box when the scene is reloaded.
     */
    void reload() {
        SpriteManager::set_visible(&selection_box, false);
        SpriteRenderer::add_sprite(&selection_box);
    }

    /**
     * Start the selection box when a multiselect action starts.
     */
    void start() {
        start_x = Mouse::get_worldX();
        start_y = Mouse::get_worldY();
        SpriteManager::set_position(&selection_box, glm::vec2{start_x, start_y });
        SpriteManager::set_visible(&selection_box, true);
        multiselect = true;
    }

    /**
     * Update the selection box during a multiselect action.
     */
    void update() {
        glm::vec2 new_scale{ Mouse::get_worldX() - start_x, Mouse::get_worldY() - start_y };
        SpriteManager::set_scale(&selection_box, new_scale);
    }

    /**
     * Hide the selection box.
     */
    void hide() {
        SpriteManager::set_visible(&selection_box, false);
        multiselect = false;
    }

    void get_selection_box(SpriteManager::sprite_manager **object) {
        *object = &selection_box;
    }

    bool is_multiselect() {
        return multiselect;
    }
}
