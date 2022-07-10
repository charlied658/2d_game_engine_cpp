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
#include "editor/mouse_drag_events.h"

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

        // Render shadows
        Shadows::render();
    }

    /**
     * User interface elements.
     */
    void imgui() {

        Highlight::get_highlighted_object(&highlighted_obj);

        ImGui::Text("Shift click down: %d", Mouse::is_shift_click_down());
        // Debug
        if (highlighted_obj) {
            ImGui::Text("Highlighted object: %s", highlighted_obj->name.c_str());
        }
        // Display highlighted game objects
        for (int i = 0; i < selected_object_count; i++) {
            ImGui::Text("Selected: %s", selected_objects[i]->name.c_str());
        }
    }
}