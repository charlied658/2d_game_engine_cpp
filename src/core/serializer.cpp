/**
 * @author Charlie Davidson 
 * Created on 7/9/22.
 */

#include "serializer.h"

#include <vector>
#include <cstdio>
#include <fstream>

#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"

#include "editor/game_object.h"
#include "editor/render/sprite_renderer.h"
#include "texture.h"
#include "editor/collision/chunk_manager.h"
#include "editor/sprite_manager.h"
#include "editor/scene.h"
#include "editor/interface/object_manager.h"
#include "util/properties.h"
#include "editor/sprite_system.h"
#include "editor/render/sprite_renderer.h"

namespace Serializer {

    static Editor::GameObject::game_object *game_objects;
    static int game_object_count;

    /**
     * Serialize the game objects and place them into an output file.
     */
    void serialize_game_objects(const std::string& filepath) {
        // Get the game objects list
        Editor::Scene::get_game_objects_list(&game_objects, &game_object_count);

        // Convert game objects list into a vector (which is readable by cereal library)
        std::vector<Editor::SpriteManager::sprite_manager> serialized_game_objects;
        for (int i = 0; i < game_object_count; i++) {
            serialized_game_objects.push_back(*game_objects[i].spr_manager);
        }

        // Open a file and write to it
        std::ofstream level_file;
        std::string path_absolute = PROJECT_PATH + filepath;
        level_file.open (path_absolute);
        {
            cereal::JSONOutputArchive out_archive(level_file); // Create an output archive
            out_archive(serialized_game_objects); // Write the data to the archive
        }
        level_file.close();
        printf("Saved level\n");
    }

    /**
     * Deserialize game objects and repopulate the game objects list.
     */
    void deserialize_game_objects(const std::string& filepath) {
        // Create a vector to receive game object data from the file
        std::vector<Editor::SpriteManager::sprite_manager> serialized_game_objects;

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
            in_archive(serialized_game_objects); // Get the data from the archive
        }
        level_file.close();

        // Clear game object lists
        Editor::Scene::clear_game_objects();
        Editor::SpriteRenderer::clear_render_batches();
        ObjectManager::reload();
        ChunkManager::reload();

        // Re-add all the game objects
        for (const auto& obj : serialized_game_objects) {
            Editor::SpriteManager::sprite_manager *spr_manager;
            Editor::SpriteSystem::add_sprite_manager(&spr_manager);
            *spr_manager = obj;
            Editor::SpriteManager::set_visible(spr_manager, true);
            ChunkManager::set_solid_block(spr_manager->grid_x, spr_manager->grid_y, true);
            spr_manager->last_grid_x =  spr_manager->grid_x;
            spr_manager->last_grid_y =  spr_manager->grid_y;
            spr_manager->last_position = spr_manager->position;
            spr_manager->sprite.texture_ID = Texture::get_texture(spr_manager->sprite.texture_filepath)->textureID;
            Editor::GameObject::game_object *go;
            Editor::Scene::add_game_object(&go);
            go->spr_manager = spr_manager;
            spr_manager->game_object = go;
            Editor::SpriteRenderer::add_sprite(go->spr_manager);
        }
        printf("Loaded level\n");
    }
}
