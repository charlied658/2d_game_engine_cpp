/**
 * @author Charlie Davidson 
 * Created on 7/15/22.
 */

#include "level_state.h"

#include "editor/scene.h"

namespace LevelState {

    static Editor::GameObject::game_object *game_objects;
    static int game_object_count;

    void init(LevelState::level_state *level_state) {
        // Populate game objects
        Editor::Scene::get_game_objects_list(&game_objects, &game_object_count);
        for (int i = 0; i < game_object_count; i++) {
            if (!game_objects[i].transient) {
                level_state->serialized_game_objects.push_back(game_objects[i]);
                if (game_objects[i].has_sprite_manager) {
                    level_state->serialized_sprite_managers.push_back(*game_objects[i].spr_manager);
                }
                if (game_objects[i].has_physics_manager) {
                    level_state->serialized_physics_managers.push_back(*game_objects[i].py_manager);
                }
                if (game_objects[i].has_behavior_manager) {
                    level_state->serialized_behavior_managers.push_back(*game_objects[i].bh_manager);
                }
            }
        }
    }
}