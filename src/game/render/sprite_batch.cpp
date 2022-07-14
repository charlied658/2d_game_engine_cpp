/**
 * @author Charlie Davidson
 * Created on 7/4/22.
 */

#include "sprite_batch.h"

#include "game/sprite_manager.h"
#include "core/shader.h"
#include "core/camera.h"

/*
 * ========= Diagram of one Quad: ==========
 *
 *               3-----------2
 *               |           |
 *               |           |
 *               |           |
 *               0-----------1
 *
 *  Triangles:
 *      0 -> 1 -> 2
 *      0 -> 2 -> 3
 *
 *  NOTE: Vertices should be given in COUNTER-CLOCKWISE
 *  order according to OpenGL
 *
 */

namespace Game {
    namespace SpriteBatch {

        static int tex_slots[] = {0, 1, 2, 3, 4, 5, 6, 7};

        // Vertex information
        static const int position_size = 2;
        static const int color_size = 4;
        static const int saturation_size = 1;
        static const int tex_coords_size = 2;
        static const int tex_id_size = 1;

        static const int position_offset = 0;
        static const int color_offset = position_size + position_offset;
        static const int saturation_offset = color_offset + color_size;
        static const int tex_coords_offset = saturation_offset + saturation_size;
        static const int tex_id_offset = tex_coords_offset + tex_coords_size;

        static const int vertex_size = position_size + color_size + saturation_size + tex_coords_size + tex_id_size;
        static const int vertex_size_bytes = sizeof(float) * vertex_size;

        /**
         * Create and initialize a render batch.
         * @param batch Sprite batch
         * @param max_batch_size Maximum number of allowed sprites
         * @param z_index Z-index of batch
         */
        void init(Game::SpriteBatch::sprite_batch *batch, int max_batch_size, int z_index) {
            batch->max_batch_size = max_batch_size;
            batch->sprite_list = new Game::SpriteManager::sprite_manager *[max_batch_size];
            batch->sprite_count = 0;
            batch->has_room = true;
            batch->vertex_data = new float[batch->max_batch_size * 4 * vertex_size];
            batch->element_data = new int[batch->max_batch_size * 6];
            batch->texture_list = new unsigned int[8];
            batch->texture_count = 1;
            batch->z_index = z_index;

            // Generate element indices
            generate_element_indices(batch);

            // Generate Vertex Array Object (VAO)
            glGenVertexArrays(1, &batch->vaoID);
            glBindVertexArray(batch->vaoID);

            // Generate Vertex Buffer Object (VBO)
            glGenBuffers(1, &batch->vboID);
            glBindBuffer(GL_ARRAY_BUFFER, batch->vboID);
            glBufferData(GL_ARRAY_BUFFER, (long) sizeof(float) * batch->max_batch_size * 4 * vertex_size,
                         batch->vertex_data, GL_DYNAMIC_DRAW);

            // Generate Element Buffer Object (EBO)
            glGenBuffers(1, &batch->eboID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->eboID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long) sizeof(float) * batch->max_batch_size * 6, batch->element_data,
                         GL_STATIC_DRAW);

            // Enable vertex attributes
            glVertexAttribPointer(0, position_size, GL_FLOAT, GL_FALSE, vertex_size_bytes,
                                  (void *) (sizeof(float) * position_offset));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, color_size, GL_FLOAT, GL_FALSE, vertex_size_bytes,
                                  (void *) (sizeof(float) * color_offset));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, saturation_size, GL_FLOAT, GL_FALSE, vertex_size_bytes,
                                  (void *) (sizeof(float) * saturation_offset));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(3, tex_coords_size, GL_FLOAT, GL_FALSE, vertex_size_bytes,
                                  (void *) (sizeof(float) * tex_coords_offset));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(4, tex_id_size, GL_FLOAT, GL_FALSE, vertex_size_bytes,
                                  (void *) (sizeof(float) * tex_id_offset));
            glEnableVertexAttribArray(4);

            // Enable alpha blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        }

        /**
         * Render vertex data for the given batch.
         * @param batch Sprite batch
         */
        void render(Game::SpriteBatch::sprite_batch *batch) {

            // Check if vertex data needs to be updated
            for (int i = 0; i < batch->sprite_count; i++) {
                if (batch->sprite_list[i]->is_dirty) {
                    generate_vertex_data(batch, i);
                    batch->sprite_list[i]->is_dirty = false;
                }
            }

            // Rebuffer vertex data (do this every frame for now)
            glBindBuffer(GL_ARRAY_BUFFER, batch->vboID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, (long) sizeof(float) * (batch->sprite_count) * 4 * vertex_size,
                            batch->vertex_data);

            // Upload view and projection matrices to the shader program
            Shader::upload_mat4("view", Camera::get_view());
            Shader::upload_mat4("projection", Camera::get_projection());

            // Upload texture slots to shader program
            Shader::upload_textures("tex_sampler", tex_slots);

            // Bind textures
            for (int i = 1; i < batch->texture_count; i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, batch->texture_list[i]);
            }

            // Draw elements
            glBindVertexArray(batch->vaoID);
            glDrawElements(GL_TRIANGLES, 6 * batch->sprite_count, GL_UNSIGNED_INT, nullptr);
        }

        /**
         * Add a sprite to the batch.
         * @param batch Sprite batch
         * @param obj Sprite
         */
        void add_sprite(Game::SpriteBatch::sprite_batch *batch, SpriteManager::sprite_manager *obj) {
            batch->sprite_list[batch->sprite_count] = obj;

            // Generate vertex data for the newly added sprite
            generate_vertex_data(batch, batch->sprite_count);

            // Increment sprite count
            batch->sprite_count++;
            if (batch->sprite_count >= batch->max_batch_size) {
                batch->has_room = false;
            }
        }

        /**
         * Remove a sprite from the batch.
         * @param batch Sprite batch
         * @param obj Sprite
         * @return Removed object successfully
         */
        bool remove_sprite(Game::SpriteBatch::sprite_batch *batch, Game::SpriteManager::sprite_manager *obj) {
            bool found_object = false;
            for (int i = 0; i < batch->sprite_count; i++) {
                if (batch->sprite_list[i] == obj) {
                    found_object = true;
                }
                if (found_object && i < batch->sprite_count - 1) {
                    batch->sprite_list[i] = batch->sprite_list[i + 1];
                }
            }
            if (found_object) {
                batch->sprite_count--;
                return true;
            }
            return false;
        }

        /**
         * Generate vertex data for the given sprite.
         * @param batch Sprite
         * @param index Index to begin adding vertex data
         */
        static void generate_vertex_data(Game::SpriteBatch::sprite_batch *batch, int index) {
            int offset = index * vertex_size * 4;
            float xAdd, yAdd;
            xAdd = 0.0f;
            yAdd = 0.0f;
            for (int j = 0; j < 4; j++) {
                if (j == 1) {
                    xAdd = 1.0f;
                } else if (j == 2) {
                    yAdd = 1.0f;
                } else if (j == 3) {
                    xAdd = 0.0f;
                }

                // ============== Upload vertex information
                // Position
                batch->vertex_data[offset + 0] =
                        (xAdd * batch->sprite_list[index]->scale.x) + batch->sprite_list[index]->position.x;
                batch->vertex_data[offset + 1] =
                        (yAdd * batch->sprite_list[index]->scale.y) + batch->sprite_list[index]->position.y;

                // Color
                batch->vertex_data[offset + 2] = batch->sprite_list[index]->out_color.x;
                batch->vertex_data[offset + 3] = batch->sprite_list[index]->out_color.y;
                batch->vertex_data[offset + 4] = batch->sprite_list[index]->out_color.z;
                batch->vertex_data[offset + 5] = batch->sprite_list[index]->out_color.w;

                // Saturation
                batch->vertex_data[offset + 6] = batch->sprite_list[index]->saturation;

                // Texture Coordinates
                batch->vertex_data[offset + 7] = (xAdd * batch->sprite_list[index]->sprite.tex_scale.x) +
                                                 batch->sprite_list[index]->sprite.tex_coords.x;
                batch->vertex_data[offset + 8] = (yAdd * batch->sprite_list[index]->sprite.tex_scale.y) +
                                                 batch->sprite_list[index]->sprite.tex_coords.y;

                // Texture ID
                if (batch->sprite_list[index]->sprite.is_null) {
                    batch->vertex_data[offset + 9] = 0.0f;
                } else {
                    batch->vertex_data[offset + 9] = (float) get_texture_slot(batch,
                                                                              batch->sprite_list[index]->sprite.texture_ID);
                }

                offset += vertex_size;
            }
        }

        /**
         * Generate element indices.
         * @param batch Sprite batch
         */
        static void generate_element_indices(Game::SpriteBatch::sprite_batch *batch) {
            int element_offset = 0;
            for (int i = 0; i < batch->max_batch_size; i++) {
                batch->element_data[i * 6 + 0] = element_offset + 0;
                batch->element_data[i * 6 + 1] = element_offset + 1;
                batch->element_data[i * 6 + 2] = element_offset + 2;
                batch->element_data[i * 6 + 3] = element_offset + 0;
                batch->element_data[i * 6 + 4] = element_offset + 2;
                batch->element_data[i * 6 + 5] = element_offset + 3;

                element_offset += 4;
            }
        }

        /**
         * Get the texture slot for the specified texture.
         * @param batch Sprite batch
         * @param texture_ID Texture ID
         * @return Texture slot
         */
        static int get_texture_slot(Game::SpriteBatch::sprite_batch *batch, unsigned int texture_ID) {
            for (int i = 0; i < batch->texture_count; i++) {
                if (texture_ID == batch->texture_list[i]) {
                    return i;
                }
            }
            // If no matching texture is found, then add it to the list.
            if (batch->texture_count < 8) {
                batch->texture_list[batch->texture_count] = texture_ID;
                int texture_slot = batch->texture_count;
                batch->texture_count++;
                return texture_slot;
            } else {
                return -1;
            }
        }

        /**
         * Check if a batch contains a texture or if it has texture room.
         * @param batch Sprite batch
         * @param texture_ID Texture ID
         * @return Contains texture or has room
         */
        bool contains_texture(Game::SpriteBatch::sprite_batch *batch, unsigned int texture_ID) {
            if (batch->texture_count < 7) {
                return true;
            }
            for (int i = 0; i < batch->texture_count; i++) {
                if (texture_ID == batch->texture_list[i]) {
                    return true;
                }
            }
            return false;
        }
    }
}