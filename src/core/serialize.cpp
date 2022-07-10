/**
 * @author Charlie Davidson 
 * Created on 7/9/22.
 */

#include "core/serialize.h"

#include <vector>
#include <cstdio>
#include <fstream>

#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"

#include "render/render.h"
#include "render/texture.h"
#include "core/game_object.h"
#include "core/scene.h"
#include "editor/object_manager.h"
#include "util/properties.h"

namespace Serialize {

    static GameObject::game_object *game_objects;
    static int game_object_count;

    /**
     * Serialize the game objects and place them into an output file.
     */
    void serialize_game_objects(const string& filepath) {
        // Get the game objects list
        Scene::get_game_objects_list(&game_objects, &game_object_count);

        // Convert game objects list into a vector (which is readable by cereal library)
        vector<GameObject::game_object> serialized_game_objects;
        for (int i = 0; i < game_object_count; i++) {
            serialized_game_objects.push_back(game_objects[i]);
        }

        // Open a file and write to it
        ofstream level_file;
        string path_absolute = PROJECT_PATH + filepath;
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
    void deserialize_game_objects(const string& filepath) {
        // Create a vector to receive game object data from the file
        vector<GameObject::game_object> serialized_game_objects;

        // Open a file a read from it
        ifstream level_file;
        string path_absolute = PROJECT_PATH + filepath;
        level_file.open (path_absolute);
        if (!level_file.is_open()) {
            printf("Error: '%s' does not exist. Please save first\n", path_absolute.c_str());
            return;
        }
        {
            cereal::JSONInputArchive in_archive(level_file); // Create an input archive
            in_archive(serialized_game_objects); // Get the data from the archive
        }
        level_file.close();

        // Clear game object lists
        Scene::clear_game_objects();
        Render::clear_render_batches();
        ObjectManager::reload();

        // Re-add all the game objects
        for (auto obj : serialized_game_objects) {
            GameObject::set_visible(&obj, true);
            GameObject::set_pickable(&obj, true);
            obj.sprite.texture_ID = Texture::get_texture(obj.sprite.texture_filepath)->textureID;
            Scene::add_game_object(&obj);
        }
        printf("Loaded level\n");
    }
}