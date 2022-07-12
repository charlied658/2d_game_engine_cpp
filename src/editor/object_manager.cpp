/**
 * @author Charlie Davidson 
 * Created on 7/9/22.
 */

#include "editor/object_manager.h"

#include "core/game_object.h"
#include "core/imgui_layer.h"
#include "editor/highlighted_objects.h"
#include "editor/mouse_events.h"
#include "editor/key_events.h"
#include "editor/selected_objects.h"
#include "editor/shadows.h"
#include "editor/copied_objects.h"
#include "editor/selection_box.h"
#include "editor/holding_object.h"
#include "editor/grid_lines.h"

namespace ObjectManager {

    static GameObject::game_object **selected_objects;
    static int selected_object_count;
    static GameObject::game_object *highlighted_obj;

    /**
     * Initialize objects.
     */
    void init() {
        Highlight::init();
        Selected::init();
        Shadows::init();
        Copy::init();
        SelectionBox::init();
        Mouse::init();
        Holding::init();
        GridLines::init();
    }

    /**
     * Reset variables when the scene is reloaded.
     */
    void reload() {
        Highlight::reload();
        Selected::reload();
        Shadows::reload();
        Copy::reload();
        SelectionBox::reload();
        Holding::reload();
    }

    /**
     * Update objects in the scene.
     */
    void update() {
        // Update highlighted objects
        Highlight::update();

        // Get updated object information
        Mouse::start_frame();
        Selected::start_frame();
        Copy::start_frame();

        // Update objects
        Mouse::update();
        Key::update();
        Holding::update();

        // Render shadows
        Shadows::render();

        // Update grid lines
        GridLines::update();
    }

    /**
     * User interface elements.
     */
    void imgui() {

        Highlight::get_highlighted_object(&highlighted_obj);

        ImGui::Text("Shift click down: %d", Mouse::is_shift_click_down());
        ImGui::Text("Dragging objects: %d", Mouse::is_dragging_objects());
        ImGui::Text("Multiselect: %d", Mouse::is_multiselect());
        ImGui::Text("Holding object: %d", Holding::is_holding());
        // Debug
        if (highlighted_obj) {
            ImGui::Text("Highlighted object: %s", highlighted_obj->name.c_str());
        }
        // Display highlighted game objects
        Selected::get_selected_objects(&selected_objects,&selected_object_count);
        for (int i = 0; i < selected_object_count; i++) {
            ImGui::Text("Selected: %s", selected_objects[i]->name.c_str());
        }
    }
}
