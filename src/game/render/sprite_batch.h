/**
 * @author Charlie Davidson 
 * Created on 7/4/22.
 */

#pragma once

#include "glad/glad.h"

#include "game/sprite_manager.h"

namespace Game {
    namespace SpriteBatch {

        struct sprite_batch {
            Game::SpriteManager::sprite_manager **sprite_list;
            int sprite_count;
            int max_batch_size;
            bool has_room;
            float *vertex_data;
            int *element_data;
            unsigned int *texture_list;
            int texture_count;
            int z_index;
            GLuint vaoID, vboID, eboID;
        };

        void init(Game::SpriteBatch::sprite_batch *batch, int max_batch_size, int z_index);

        void render(Game::SpriteBatch::sprite_batch *batch);

        void add_sprite(Game::SpriteBatch::sprite_batch *batch, Game::SpriteManager::sprite_manager *obj);

        bool remove_sprite(Game::SpriteBatch::sprite_batch *batch, Game::SpriteManager::sprite_manager *obj);

        static void generate_vertex_data(Game::SpriteBatch::sprite_batch *batch, int index);

        static void generate_element_indices(Game::SpriteBatch::sprite_batch *batch);

        static int get_texture_slot(Game::SpriteBatch::sprite_batch *batch, unsigned int texture_ID);

        bool contains_texture(Game::SpriteBatch::sprite_batch *batch, unsigned int texture_ID);

    }
}