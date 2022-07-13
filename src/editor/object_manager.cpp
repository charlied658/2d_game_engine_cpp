/**
 * @author Charlie Davidson 
 * Created on 7/9/22.
 */

#include "editor/object_manager.h"

#include "core/sprite_manager.h"
#include "core/imgui_layer.h"
#include "core/scene.h"
#include "editor/highlighted_objects.h"
#include "editor/mouse_events.h"
#include "editor/key_events.h"
#include "editor/selected_objects.h"
#include "editor/shadows.h"
#include "editor/copied_objects.h"
#include "editor/selection_box.h"
#include "editor/holding_object.h"
#include "editor/grid_lines.h"
#include "editor/preview_objects.h"
#include "editor/drag_objects.h"
#include "editor/collision/chunk_manager.h"
#include "core/render/sprite_renderer.h"

namespace ObjectManager {

    static SpriteManager::sprite_manager **highlighted_objects;
    static int highlighted_object_count;
    static SpriteManager::sprite_manager **selected_objects;
    static int selected_object_count;
    static SpriteManager::sprite_manager **copied_objects;
    static int copied_object_count;
    static SpriteManager::sprite_manager *highlighted_obj;
    static SpriteManager::sprite_manager *game_objects;
    static int game_object_count;

    /**
     * Initialize objects.
     */
    void init() {
        Highlight::init();
        Selected::init();
        Shadows::init();
        Preview::init();
        Copy::init();
        SelectionBox::init();
        Drag::init();
        Holding::init();
        GridLines::init();
        ChunkManager::init();
    }

    /**
     * Reset variables when the scene is reloaded.
     */
    void reload() {
        Highlight::reload();
        Selected::reload();
        Shadows::reload();
        Preview::reload();
        Copy::reload();
        SelectionBox::reload();
        Holding::reload();
    }

    /**
     * Update objects in the scene.
     */
    void update() {
        Scene::get_game_objects_list(&game_objects, &game_object_count);

        // Reset z-index of objects
        ObjectManager::reset_z_index();

        // Update highlighted objects
        Highlight::update();

        // Get updated object information
        Drag::start_frame();
        Selected::start_frame();
        Copy::start_frame();

        // Update objects
        Mouse::update();
        Key::update();
        Holding::update();

        // Render visual elements
        Shadows::render();
        Preview::update();
        GridLines::update();

        // Update Z-index
        ObjectManager::update_z_index();
    }

    /**
     * Reset z-index of all game objects.
     */
    void reset_z_index() {
        for (int i = 0; i < game_object_count; i++) {
            game_objects[i].new_z_index = 0;
        }
    }

    /**
     * Update the z-index of any game object that needs to be updated.
     */
    void update_z_index() {
        for (int i = 0; i < game_object_count; i++) {
            if (game_objects[i].new_z_index != game_objects[i].z_index) {
                SpriteRenderer::re_add_sprite(&game_objects[i]);
            }
        }
    }

    /**
     * User interface elements. Mostly for debugging.
     */
    void imgui() {
        // Get updated information
        Highlight::get_highlighted_object(&highlighted_obj);
        Highlight::get_highlighted_objects(&highlighted_objects, &highlighted_object_count);
        Selected::get_selected_objects(&selected_objects,&selected_object_count);
        Copy::get_copied_objects(&copied_objects, &copied_object_count);
        Scene::get_game_objects_list(&game_objects, &game_object_count);

        // Mouse information
        ImGui::Text("Mouse down: %d", Mouse::is_mouse_down());
        ImGui::Text("Dragging objects: %d", Drag::is_dragging_objects());
        ImGui::Text("Multiselect: %d", SelectionBox::is_multiselect());
        ImGui::Text("Holding object: %d", Holding::is_holding());

        // Object information
        ImGui::Text("Game object count: %d", game_object_count);
        ImGui::Text("Highlighted object count: %d", highlighted_object_count);
        ImGui::Text("Selected object count: %d", selected_object_count);
        ImGui::Text("Copied object count: %d", copied_object_count);
    }
}
