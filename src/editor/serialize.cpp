/**
 * @author Charlie Davidson 
 * Created on 7/9/22.
 */

#include "editor/serialize.h"

#include <vector>
#include <cstdio>
#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include "render/render.h"
#include "core/game_object.h"
#include "core/scene.h"
#include "editor/select_objects.h"
#include "util/properties.h"

namespace Serialize {
    /**
     * Serialize the game objects and place them into an output file.
     */
    void serialize_game_objects(const string& filepath) {

        GameObject::game_object *game_objects;
        int game_object_count;
        Scene::get_game_objects_list(&game_objects, &game_object_count);

        // Convert game objects list into a vector (readable by cereal library)
        vector<GameObject::game_object> serialized_game_objects;

        for (int i = 0; i < game_object_count; i++) {
            serialized_game_objects.push_back(game_objects[i]);
        }

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
        vector<GameObject::game_object> serialized_game_objects;

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

        SelectObjects::init();

        // Re-add all the game objects
        for (int i = 0; i < serialized_game_objects.size(); i++) {
            GameObject::game_object obj = serialized_game_objects[i];
            obj.visible = true;
            obj.pickable = true;
            GameObject::update_color(&obj);
            Scene::add_game_object(&obj);
        }

        printf("Loaded level\n");
    }
}
