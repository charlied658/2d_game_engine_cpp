/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "selection_box.h"

#include "editor/game_object.h"
#include "editor/render/sprite_renderer.h"
#include "mouse_events.h"
#include "editor/sprite_system.h"
#include "editor/scene.h"

namespace SelectionBox {

    static Sprite::sprite sprite_null;
    static Editor::GameObject::game_object *selection_box;
    static double start_x, start_y;
    static bool multiselect;

    /**
     * Initialize selection box.
     */
    void init() {

        Editor::Scene::init_transient_game_object(&selection_box);
        Editor::GameObject::init_transient(selection_box);
        Editor::SpriteSystem::init_transient_sprite_manager(&selection_box->spr_manager);
        Editor::GameObject::init_sprite_manager(selection_box, selection_box->spr_manager);

        // Generate null sprite (Renders as a solid color)
        Sprite::get_null_sprite(&sprite_null);

        // Generate selection box (Appears when you click and drag to multiselect)
        Editor::SpriteManager::init(selection_box->spr_manager, glm::vec2 {1.0f, 1.5f}, glm::vec2 {1.0f, 1.0f}, 2, &sprite_null);
        Editor::SpriteManager::set_color(selection_box->spr_manager, glm::vec4 {1.0f, 1.0f, 0.0f, 0.2f});
        selection_box->spr_manager->pickable = false;
        Editor::SpriteManager::set_visible(selection_box->spr_manager, false);
        Editor::SpriteRenderer::add_transient_sprite(selection_box->spr_manager);
    }

    /**
     * Reset selection box when the scene is reloaded.
     */
    void reload() {
        Editor::SpriteManager::set_visible(selection_box->spr_manager, false);
        Editor::SpriteRenderer::add_sprite(selection_box->spr_manager);
    }

    /**
     * Start the selection box when a multiselect action starts.
     */
    void start() {
        start_x = Mouse::get_worldX();
        start_y = Mouse::get_worldY();
        Editor::SpriteManager::set_position(selection_box->spr_manager, glm::vec2{start_x, start_y });
        Editor::SpriteManager::set_visible(selection_box->spr_manager, true);
        multiselect = true;
    }

    /**
     * Update the selection box during a multiselect action.
     */
    void update() {
        glm::vec2 new_scale{ Mouse::get_worldX() - start_x, Mouse::get_worldY() - start_y };
        Editor::SpriteManager::set_scale(selection_box->spr_manager, new_scale);
    }

    /**
     * Hide the selection box.
     */
    void hide() {
        Editor::SpriteManager::set_visible(selection_box->spr_manager, false);
        multiselect = false;
    }

    void get_selection_box(Editor::GameObject::game_object **object) {
        *object = selection_box;
    }

    bool is_multiselect() {
        return multiselect;
    }
}
