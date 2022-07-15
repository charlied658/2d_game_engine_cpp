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
#include "editor/serialization/level_state.h"

namespace Serializer {

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

        // Re-add all the game objects
        for (const auto& obj : level_state.serialized_sprite_managers) {
            Editor::SpriteManager::sprite_manager *spr_manager;
            Editor::SpriteSystem::init_sprite_manager(&spr_manager);
            *spr_manager = obj;
            Editor::SpriteManager::set_visible(spr_manager, true);
            ChunkManager::set_solid_block(spr_manager->grid_x, spr_manager->grid_y, true);
            spr_manager->last_grid_x =  spr_manager->grid_x;
            spr_manager->last_grid_y =  spr_manager->grid_y;
            spr_manager->last_position = spr_manager->position;
            spr_manager->sprite.texture_ID = Texture::get_texture(spr_manager->sprite.texture_filepath)->textureID;
            Editor::GameObject::game_object *go;
            Editor::Scene::init_game_object(&go);
            go->spr_manager = spr_manager;
            spr_manager->game_object = go;
            Editor::SpriteRenderer::add_sprite(go->spr_manager);
        }
        printf("Loaded level\n");
    }
}
