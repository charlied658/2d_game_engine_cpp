/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#include <cstdio>
#include "chunk_manager.h"

#include "core/game_object.h"
#include "core/scene.h"
#include "collision_math.h"

namespace ChunkManager {

    static Chunk::chunk *chunk_list;
    static int chunk_count;
    static const int chunk_size = 16;

    static GameObject::game_object *game_objects;
    static int game_object_count;

    /**
     * Initialize chunk list.
     */
    void init() {
        chunk_list = new Chunk::chunk[100];
        chunk_count = 0;
    }

    /**
     * Reset variables when the scene is reloaded.
     */
    void reload() {
        // Free memory
        for (int i = 0; i < chunk_count; i++) {
            for (int j = 0; j < chunk_size; j++) {
                delete chunk_list[i].grid[j];
            }
            delete chunk_list[i].grid;
        }
        chunk_count = 0;

        // Re-add chunk information
        Scene::get_game_objects_list(&game_objects, &game_object_count);
        for (int i = 0; i < game_object_count; i++) {
            ChunkManager::set_solid_block(game_objects[i].grid_x, game_objects[i].grid_y, true);
        }
    }

    /**
     * Check if a block is solid.
     * @param x X position
     * @param y Y position
     * @return True if block is solid
     */
    bool is_solid_block(int x, int y) {
        // Loop through the list of chunks to find the chunk that contains the x,y coordinates
        for (int i = 0; i < chunk_count; i++) {
            if (x >= chunk_list[i].x && x < chunk_list[i].x + chunk_size &&
                    y >= chunk_list[i].y && y < chunk_list[i].y + chunk_size) {
                int x_mod = Math::i_mod(x, chunk_size);
                int y_mod = Math::i_mod(y, chunk_size);
                return Chunk::is_solid_block(&chunk_list[i], x_mod ,y_mod);
            }
        }
        return false;
    }

    /**
     * Set a chunk unit to be solid or not.
     * @param chunk Chunk reference
     * @param x X position
     * @param y Y position
     * @param state True if chunk unit is to be solid
     */
    void set_solid_block(int x, int y, bool state) {
        // Loop through the list of chunks to find the chunk that contains the x,y coordinates
        for (int i = 0; i < chunk_count; i++) {
            if (x >= chunk_list[i].x && x < chunk_list[i].x + chunk_size &&
                y >= chunk_list[i].y && y < chunk_list[i].y + chunk_size) {
                int x_mod = Math::i_mod(x, chunk_size);
                int y_mod = Math::i_mod(y, chunk_size);
                //printf("Accessed chunk %d, %d\n",chunk_list[i].x, chunk_list[i].y);
                Chunk::set_solid_block(&chunk_list[i], x_mod ,y_mod, state);
                return;
            }
        }
        //If no chunk exists, create a new one
        int x_mod = Math::i_mod(x, chunk_size);
        int y_mod = Math::i_mod(y, chunk_size);
        Chunk::init(&chunk_list[chunk_count], x - x_mod, y - y_mod, chunk_size);
        Chunk::set_solid_block(&chunk_list[chunk_count], x_mod ,y_mod, state);
        printf("Created new chunk %d, %d\n",chunk_list[chunk_count].x, chunk_list[chunk_count].y);
        chunk_count++;
    }
}
