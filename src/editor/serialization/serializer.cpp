/**
 * @author Charlie Davidson 
 * Created on 7/9/22.
 */

#include "serializer.h"

#include <cstdio>
#include <fstream>

#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/vector.hpp"

#include "editor/game_object.h"
#include "editor/render/sprite_renderer.h"
#include "core/texture.h"
#include "editor/collision/chunk_manager.h"
#include "editor/sprite_manager.h"
#include "editor/scene.h"
#include "editor/interface/object_manager.h"
#include "util/properties.h"
#include "editor/sprite_system.h"
#include "editor/physics_system.h"
#include "editor/behavior_system.h"
#include "editor/serialization/level_state.h"

namespace Serializer {

    static Editor::GameObject::game_object *game_objects_list;
    static int game_objects_count;
    static Editor::SpriteManager::sprite_manager *sprite_manager_list;
    static int sprite_manager_count;
    static Editor::PhysicsManager::physics_manager *physics_manager_list;
    static int physics_manager_count;
    static Editor::BehaviorManager::behavior_manager *behavior_manager_list;
    static int behavior_manager_count;

    /**
     * Serialize the game objects and place them into an output file.
     */
    void serialize_game_objects(const std::string& filepath) {
        // Get the level state
        LevelState::level_state level_state;
        LevelState::init(&level_state);

        // Open a file and write to it
        std::ofstream level_file;
        std::string path_absolute = PROJECT_PATH + filepath;
        level_file.open (path_absolute);
        {
            cereal::JSONOutputArchive out_archive(level_file); // Create an output archive
            out_archive(level_state); // Write the data to the archive
        }
        level_file.close();
        printf("Saved level\n");
    }

    /**
     * Deserialize game objects and repopulate the game objects list.
     */
    void deserialize_game_objects(const std::string& filepath) {
        // Create a level state to receive object data from the file
        LevelState::level_state level_state;

        // Open a file a read from it
        std::ifstream level_file;
        std::string path_absolute = PROJECT_PATH + filepath;
        level_file.open (path_absolute);
        if (!level_file.is_open()) {
            Editor::Scene::save_level();
            return;
        }
        {
            cereal::JSONInputArchive in_archive(level_file); // Create an input archive
            in_archive(level_state); // Get the data from the archive
        }
        level_file.close();

        // Clear game object lists
        Editor::Scene::clear_game_objects();
        Editor::SpriteRenderer::clear_render_batches();
        ObjectManager::reload();
        ChunkManager::reload();

        // Re-add game objects
        for (auto & serialized_game_object : level_state.serialized_game_objects) {
            Editor::GameObject::game_object *obj;
            Editor::Scene::init_game_object(&obj);
            *obj = serialized_game_object;
            //printf("Added game object with ID %d\n", obj->id);
        }

        // Re-add sprite managers
        for (const auto& serialized_sprite_manager : level_state.serialized_sprite_managers) {
            Editor::SpriteManager::sprite_manager *spr_manager;
            Editor::SpriteSystem::init_sprite_manager(&spr_manager);
            *spr_manager = serialized_sprite_manager;
            Editor::SpriteManager::set_visible(spr_manager, true);
            ChunkManager::set_solid_block(spr_manager->grid_x, spr_manager->grid_y, true);
            spr_manager->last_grid_x =  spr_manager->grid_x;
            spr_manager->last_grid_y =  spr_manager->grid_y;
            spr_manager->last_position = spr_manager->position;
            spr_manager->sprite.texture_ID = Texture::get_texture(spr_manager->sprite.texture_filepath)->textureID;
        }

        // Re-add physics managers
        for (auto & serialized_physics_manager : level_state.serialized_physics_managers) {
            Editor::PhysicsManager::physics_manager *py_manager;
            Editor::PhysicsSystem::init_physics_manager(&py_manager);
            *py_manager = serialized_physics_manager;
        }

        // Re-add behavior managers
        for (auto & serialized_behavior_manager : level_state.serialized_behavior_managers) {
            Editor::BehaviorManager::behavior_manager *bh_manager;
            Editor::BehaviorSystem::init_behavior_manager(&bh_manager);
            *bh_manager = serialized_behavior_manager;
        }

        // Link game objects with their components
        Editor::Scene::get_game_objects_list(&game_objects_list,&game_objects_count);
        Editor::SpriteSystem::get_sprite_manager_list(&sprite_manager_list,&sprite_manager_count);
        Editor::PhysicsSystem::get_physics_manager_list(&physics_manager_list,&physics_manager_count);
        Editor::BehaviorSystem::get_behavior_manager_list(&behavior_manager_list,&behavior_manager_count);
        for (int i = 0; i < game_objects_count; i++) {
            // Sprite manager
            for (int j = 0; j < sprite_manager_count; j++) {
                if (sprite_manager_list[j].id == game_objects_list[i].id) {
                    //printf("Found sprite renderer for object %d\n", game_objects_list[i].id);
                    Editor::GameObject::init_sprite_manager(&game_objects_list[i], &sprite_manager_list[j]);
                    Editor::SpriteRenderer::add_sprite(game_objects_list[i].spr_manager);
                    break;
                }
            }
            // Physics manager
            for (int j = 0; j < physics_manager_count; j++) {
                if (physics_manager_list[j].id == game_objects_list[i].id) {
                    Editor::GameObject::init_physics_manager(&game_objects_list[i], &physics_manager_list[j]);
                    break;
                }
            }
            // Behavior manager
            for (int j = 0; j < behavior_manager_count; j++) {
                if (behavior_manager_list[j].id == game_objects_list[i].id) {
                    Editor::GameObject::init_behavior_manager(&game_objects_list[i], &behavior_manager_list[j]);
                    break;
                }
            }
        }

        printf("Loaded level\n");
    }
}
