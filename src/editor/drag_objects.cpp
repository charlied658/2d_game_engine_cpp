/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#include "editor/drag_objects.h"

#include "core/game_object.h"
#include "core/mouse_listener.h"
#include "render/chunk_manager.h"
#include "editor/selected_objects.h"
#include "editor/copied_objects.h"
#include "util/collision_math.h"

namespace Drag {

    static bool drag_objects;
    static bool invalid_placement;
    static double start_x, start_y;
    static glm::vec2 *obj_start_pos;

    static GameObject::game_object **selected_objects;
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
            // TODO: This line is problematic when copying objects
            ChunkManager::set_solid_block(selected_objects[i]->grid_x, selected_objects[i]->grid_y, false);
            obj_start_pos[i] = selected_objects[i]->position;
        }
    }

    void update_drag() {
        invalid_placement = false;

        // If there are multiple selected objects, dragging one will move all of them
        for (int i = 0; i < selected_object_count; i++) {
            glm::vec2 grid_position = Math::grid_position(selected_objects[i]->position);
            int grid_x = (int) (grid_position.x / 0.25f);
            int grid_y = (int) (grid_position.y / 0.25f);
            GameObject::set_grid_position(selected_objects[i], grid_x, grid_y);
            if (ChunkManager::is_solid_block(grid_x, grid_y)) {
                invalid_placement = true;
            }
            GameObject::set_dragging(selected_objects[i], true);
            GameObject::set_position(selected_objects[i], glm::vec2{
                    Mouse::get_worldX() - start_x + obj_start_pos[i].x,
                    Mouse::get_worldY() - start_y + +obj_start_pos[i].y});
        }
        if (invalid_placement) {
            for (int i = 0; i < selected_object_count; i++) {
                GameObject::update_color(selected_objects[i]);
            }
        }
    }

    void end_drag() {
        if (!invalid_placement) {
            // If the block placement is valid, snap objects to grid
            Drag::snap_to_grid();
        } else {
            // Otherwise, snap objects to their last location
            Drag::snap_to_last_position();
        }
        drag_objects = false;
    }

    void snap_to_grid() {
        for (int i = 0; i < selected_object_count; i++) {
            glm::vec2 grid_position = Math::grid_position(selected_objects[i]->position);
            int grid_x = (int) (grid_position.x / 0.25f);
            int grid_y = (int) (grid_position.y / 0.25f);
            GameObject::set_position(selected_objects[i], grid_position);
            selected_objects[i]->last_position = grid_position;
            GameObject::set_grid_position(selected_objects[i], grid_x, grid_y);
            GameObject::set_last_grid_position(selected_objects[i], grid_x, grid_y);
            ChunkManager::set_solid_block(grid_x, grid_y, true);
        }
    }

    void snap_to_last_position() {
        for (int i = 0; i < selected_object_count; i++) {
            GameObject::set_position(selected_objects[i], selected_objects[i]->last_position);
            selected_objects[i]->grid_x = selected_objects[i]->last_grid_x;
            selected_objects[i]->grid_y = selected_objects[i]->last_grid_y;
            ChunkManager::set_solid_block(selected_objects[i]->grid_x, selected_objects[i]->grid_y, true);
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
