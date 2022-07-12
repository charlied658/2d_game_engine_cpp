/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/selection_box.h"

#include "render/sprite_renderer.h"

namespace SelectionBox {

    static Sprite::sprite sprite_null;
    static GameObject::game_object selection_box;

    /**
     * Initialize selection box.
     */
    void init() {
        // Generate null sprite (Renders as a solid color)
        Sprite::get_null_sprite(&sprite_null);

        // Generate selection box (Appears when you click and drag to multiselect)
        GameObject::init(&selection_box, "selection_box", glm::vec2 {1.0f, 1.5f}, glm::vec2 {1.0f, 1.0f}, 2, &sprite_null);
        GameObject::set_color(&selection_box, glm::vec4 {1.0f, 1.0f, 0.0f, 0.2f});
        GameObject::set_pickable(&selection_box, false);
        GameObject::set_visible(&selection_box, false);
        SpriteRenderer::add_game_object(&selection_box);
    }

    /**
     * Reset selection box when the scene is reloaded.
     */
    void reload() {
        GameObject::set_visible(&selection_box, false);
        SpriteRenderer::add_game_object(&selection_box);
    }

    /**
     * Get the selection box.
     * @param object Selection box
     */
    void get_selection_box(GameObject::game_object **object) {
        *object = &selection_box;
    }
}
