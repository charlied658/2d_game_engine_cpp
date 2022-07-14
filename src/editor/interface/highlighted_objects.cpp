/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "highlighted_objects.h"

#include "editor/scene.h"
#include "core/imgui_layer.h"
#include "editor/render/sprite_renderer.h"
#include "selected_objects.h"
#include "drag_objects.h"
#include "selection_box.h"

namespace Highlight {

    static Editor::SpriteManager::sprite_manager **highlighted_objects;
    static int highlighted_object_count;
    static Editor::SpriteManager::sprite_manager *highlighted_obj;

    static Editor::SpriteManager::sprite_manager *game_objects;
    static int game_object_count;
    static Editor::SpriteManager::sprite_manager **selected_objects;
    static int selected_object_count;
    static Editor::SpriteManager::sprite_manager *selection_box;

    /**
     * Initialize highlighted objects.
     */
    void init() {
        highlighted_objects = new Editor::SpriteManager::sprite_manager *[1000];
        highlighted_object_count = 0;
        highlighted_obj = nullptr;
    }

    /**
     * Reset variables when the scene is reloaded.
     */
    void reload() {
        highlighted_object_count = 0;
    }

    /**
     * Update highlighted objects.
     */
    void update() {
        Editor::Scene::get_game_objects_list(&game_objects, &game_object_count);
        SelectionBox::get_selection_box(&selection_box);
        Highlight::reset();
        Highlight::highlight_objects();
    }

    /**
     * Reset all objects to not be highlighted. Do this at the start of each frame.
     */
    static void reset() {
        // Set all game objects to not be highlighted or dragging
        for (int i = 0; i < game_object_count; i++) {
            Editor::SpriteManager::set_highlighted(&game_objects[i], false);
            Editor::SpriteManager::set_dragging(&game_objects[i], false);
        }
        // Only set the highlighted object to null if objects are not being dragged
        if (!Drag::is_dragging_objects()) {
            highlighted_obj = nullptr;
        }
    }

    /**
     * Highlight objects, either under the mouse pointer or touching the selection box.
     */
    static void highlight_objects() {
        // Check if the mouse is being dragged or not
        if (SelectionBox::is_multiselect()) {
            // Highlight game objects intersecting with the selection box
            Editor::SpriteRenderer::highlight_sprites(highlighted_objects, &highlighted_object_count, selection_box);
        } else {
            // Highlight the object that the mouse cursor is over
            if (!ImGuiLayer::want_mouse_capture() && !Drag::is_dragging_objects()) {
                Editor::SpriteRenderer::highlight_sprite(&highlighted_obj);
            }
            // Highlight the other selected objects
            if (highlighted_obj && highlighted_obj->selected) {
                Selected::get_selected_objects(&selected_objects, &selected_object_count);
                for (int i = 0; i < selected_object_count; i++) {
                    Editor::SpriteManager::set_highlighted(selected_objects[i], true);
                }
            }
        }
    }

    void get_highlighted_object(Editor::SpriteManager::sprite_manager **object) {
        *object = highlighted_obj;
    }

    void get_highlighted_objects(Editor::SpriteManager::sprite_manager ***objects, int *object_count) {
        *objects = highlighted_objects;
        *object_count = highlighted_object_count;
    }

    bool is_highlighted() {
        return highlighted_obj != nullptr;
    }

    bool is_selected() {
        return highlighted_obj != nullptr && highlighted_obj->selected;
    }
}
