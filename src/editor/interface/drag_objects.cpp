/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#include "drag_objects.h"

#include "editor/sprite_manager.h"
#include "core/mouse_listener.h"
#include "editor/collision/chunk_manager.h"
#include "selected_objects.h"
#include "copied_objects.h"
#include "editor/collision/collision_math.h"

namespace Drag {

    static bool drag_objects;
    static bool invalid_placement;
    static double start_x, start_y;
    static glm::vec2 *obj_start_pos;

    static Editor::GameObject::game_object **selected_objects;
    static int selected_object_count;

    /**
     * Initialize drag objects.
     */
    void init() {
        obj_start_pos = new glm::vec2[1000];
    }

    /**
     * Get updated object information.
     */
    void start_frame() {
        Selected::get_selected_objects(&selected_objects, &selected_object_count);
    }

    void start_drag() {
        drag_objects = true;
        start_x = Mouse::get_worldX();
        start_y = Mouse::get_worldY();
        for (int i = 0; i < selected_object_count; i++) {
            if (!Copy::get_copied()) {
                ChunkManager::set_solid_block(selected_objects[i]->spr_manager->grid_x, selected_objects[i]->spr_manager->grid_y, false);
            }
            obj_start_pos[i] = selected_objects[i]->spr_manager->position;
        }
    }

    void update_drag() {
        invalid_placement = false;

        // If there are multiple selected objects, dragging one will move all of them
        for (int i = 0; i < selected_object_count; i++) {
            glm::vec2 grid_position = Math::grid_position(selected_objects[i]->spr_manager->position);
            int grid_x = (int) (grid_position.x / 0.25f);
            int grid_y = (int) (grid_position.y / 0.25f);
            selected_objects[i]->spr_manager->grid_x = grid_x;
            selected_objects[i]->spr_manager->grid_y = grid_y;
            if (ChunkManager::is_solid_block(grid_x, grid_y)) {
                invalid_placement = true;
            }
            Editor::SpriteManager::set_dragging(selected_objects[i]->spr_manager, true);
            Editor::SpriteManager::set_position(selected_objects[i]->spr_manager, glm::vec2{
                    Mouse::get_worldX() - start_x + obj_start_pos[i].x,
                    Mouse::get_worldY() - start_y + +obj_start_pos[i].y});
        }
        if (invalid_placement) {
            for (int i = 0; i < selected_object_count; i++) {
                Editor::SpriteManager::update_color(selected_objects[i]->spr_manager);
            }
        }
    }

    void end_drag() {
        if (!invalid_placement) {
            // If the block placement is valid, snap objects to grid
            if (Copy::get_copied()) {
                Copy::set_copied(false);
            }
            Drag::snap_to_grid();
        } else {
            // Otherwise, snap objects to their last location
            if (!Copy::get_copied()) {
                Drag::snap_to_last_position();
            } else {
                Copy::delete_objects();
                Copy::set_copied(false);
                invalid_placement = false;
            }
        }
        drag_objects = false;
    }

    void snap_to_grid() {
        for (int i = 0; i < selected_object_count; i++) {
            glm::vec2 grid_position = Math::grid_position(selected_objects[i]->spr_manager->position);
            int grid_x = (int) (grid_position.x / 0.25f);
            int grid_y = (int) (grid_position.y / 0.25f);
            Editor::SpriteManager::set_position(selected_objects[i]->spr_manager, grid_position);
            selected_objects[i]->spr_manager->last_position = grid_position;
            selected_objects[i]->spr_manager->grid_x = grid_x;
            selected_objects[i]->spr_manager->grid_y = grid_y;
            selected_objects[i]->spr_manager->last_grid_x = grid_x;
            selected_objects[i]->spr_manager->last_grid_y = grid_y;
            ChunkManager::set_solid_block(grid_x, grid_y, true);
        }
    }

    void snap_to_last_position() {
        for (int i = 0; i < selected_object_count; i++) {
            Editor::SpriteManager::set_position(selected_objects[i]->spr_manager, selected_objects[i]->spr_manager->last_position);
            selected_objects[i]->spr_manager->grid_x = selected_objects[i]->spr_manager->last_grid_x;
            selected_objects[i]->spr_manager->grid_y = selected_objects[i]->spr_manager->last_grid_y;
            ChunkManager::set_solid_block(selected_objects[i]->spr_manager->grid_x, selected_objects[i]->spr_manager->grid_y, true);
        }
        invalid_placement = false;
    }

    bool is_dragging_objects() {
        return drag_objects;
    }

    bool is_invalid_placement() {
        return invalid_placement;
    }
}
