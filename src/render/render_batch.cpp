/**
 * @author Charlie Davidson 
 * Created on 7/4/22.
 */

#include "render/render_batch.h"

#include <cstdio>

#include "core/game_object.h"
#include "render/shader.h"
#include "core/camera.h"

struct vertex {
    float x, y;       // Position
    float r, g, b, a; // Color
    float u, v;       // Texture coordinates
    float t;          // Texture ID
};

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

namespace RenderBatch {

    static int tex_slots[] = {0,1,2,3,4,5,6,7};

    // Vertex information
    static const int position_size = 2;
    static const int color_size = 4;
    static const int tex_coords_size = 2;
    static const int tex_id_size = 1;

    static const int color_offset = position_size;
    static const int tex_coords_offset = color_offset + color_size;
    static const int tex_id_offset = tex_coords_offset + tex_coords_size;

    static const int vertex_size = position_size + color_size + tex_coords_size + tex_id_size;

    /**
     * Create and initialize a render batch.
     * @param batch Render batch reference
     * @param max_batch_size Maximum number of allowed game objects
     */
    void init(render_batch *batch, int max_batch_size, int z_index) {
        batch->max_batch_size = max_batch_size;
        batch->game_object_list = new GameObject::game_object *[max_batch_size];
        batch->game_object_count = 0;
        batch->has_room = true;
        batch->vertex_data = new float[batch->max_batch_size * 4 * vertex_size];
        batch->element_data = new int[batch->max_batch_size * 6];
        batch->texture_list = new unsigned int[8];
        batch->texture_count = 0;
        batch->z_index = z_index;

        // Generate element indices
        generate_element_indices(batch);

        // Generate Vertex Array Object (VAO)
        glGenVertexArrays(1, &batch->vaoID);
        glBindVertexArray(batch->vaoID);

        // Generate Vertex Buffer Object (VBO)
        glGenBuffers(1, &batch->vboID);
        glBindBuffer(GL_ARRAY_BUFFER, batch->vboID);
        glBufferData(GL_ARRAY_BUFFER, (long) sizeof(float) * batch->max_batch_size * 4 * vertex_size, batch->vertex_data, GL_DYNAMIC_DRAW);

        // Generate Element Buffer Object (EBO)
        glGenBuffers(1, &batch->eboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->eboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long) sizeof(float) * batch->max_batch_size * 6, batch->element_data, GL_STATIC_DRAW);

        // Enable vertex attributes
        glVertexAttribPointer(0, position_size, GL_FLOAT, GL_FALSE, sizeof(vertex),nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, color_size, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * color_offset));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, tex_coords_size, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * tex_coords_offset));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, tex_id_size, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * tex_id_offset));
        glEnableVertexAttribArray(3);

        // Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    }

    /**
     * Render vertex data for the given batch.
     * @param batch Render batch reference
     */
    void render(render_batch *batch) {

        // Check if vertex data needs to be updated
        for (int i = 0; i < batch->game_object_count; i++) {
            if (batch->game_object_list[i]->is_dirty) {
                generate_vertex_data(batch, i);
                batch->game_object_list[i]->is_dirty = false;
            }
        }

        // Rebuffer vertex data (do this every frame for now)
        glBindBuffer(GL_ARRAY_BUFFER, batch->vboID);
        glBufferSubData(GL_ARRAY_BUFFER,0, (long) sizeof(float) * (batch->game_object_count) * 4 * vertex_size, batch->vertex_data);

        // Upload view and projection matrices to the shader program
        Shader::upload_mat4("view", Camera::get_view());
        Shader::upload_mat4("projection", Camera::get_projection());

        // Upload texture slots to shader program
        Shader::upload_textures("tex_sampler", tex_slots);

        // Bind textures
        for (int i = 0; i < batch->texture_count; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, batch->texture_list[i]);
        }

        // Draw elements
        glBindVertexArray(batch->vaoID);
        glDrawElements(GL_TRIANGLES, 6 * batch->game_object_count, GL_UNSIGNED_INT, nullptr);
    }

    /**
     * Add a game object to the batch.
     * @param batch Render batch reference
     * @param obj Game object to be added
     */
    void add_game_object(render_batch *batch, GameObject::game_object *obj) {
        batch->game_object_list[batch->game_object_count] = obj;

        // Generate vertex data for the newly added game object
        generate_vertex_data(batch, batch->game_object_count);

        // Increment game object count
        batch->game_object_count++;
        if (batch->game_object_count >= batch->max_batch_size) {
            batch->has_room = false;
        }
    }

    /**
     * Generate vertex data for the given game object.
     * @param batch Render batch reference
     * @param index Index to begin adding vertex data
     */
    static void generate_vertex_data(render_batch *batch, int index) {
        int offset = index * vertex_size * 4;
        float xAdd, yAdd;
        xAdd = 0.0f;
        yAdd = 0.0f;
        for (int j = 0; j < 4; j++) {
            if (j == 1) {
                xAdd = 1.0f;
            } else if (j == 2) {
                yAdd = 1.0f;
            } else if (j == 3){
                xAdd = 0.0f;
            }

            // ============== Upload vertex information
            // Position
            batch->vertex_data[offset + 0] = (xAdd * batch->game_object_list[index]->x_scale) + batch->game_object_list[index]->x_pos;
            batch->vertex_data[offset + 1] = (yAdd * batch->game_object_list[index]->y_scale) + batch->game_object_list[index]->y_pos;

            // Color
            batch->vertex_data[offset + 2] = batch->game_object_list[index]->r;
            batch->vertex_data[offset + 3] = batch->game_object_list[index]->g;
            batch->vertex_data[offset + 4] = batch->game_object_list[index]->b;
            batch->vertex_data[offset + 5] = batch->game_object_list[index]->a;

            // Texture Coordinates
            batch->vertex_data[offset + 6] = (xAdd * batch->game_object_list[index]->sprite.x_tex_scale) + batch->game_object_list[index]->sprite.x_tex0;
            batch->vertex_data[offset + 7] = (yAdd * batch->game_object_list[index]->sprite.y_tex_scale) + batch->game_object_list[index]->sprite.y_tex0;

            // Texture ID
            batch->vertex_data[offset + 8] = (float) get_texture_slot(batch, batch->game_object_list[index]->sprite.texture_ID);

            offset += 9;
        }
    }

    /**
     * Generate element indices.
     * @param batch Render batch reference
     */
    static void generate_element_indices(render_batch *batch) {
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
     * @param batch Render batch reference
     * @param texture_ID Texture ID
     * @return Texture slot
     */
    static int get_texture_slot(render_batch *batch, unsigned int texture_ID) {
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
     * @param batch Render batch reference
     * @param texture_ID Texture ID
     * @return True if the batch contains a texture or if it has space to add a new texture
     */
    bool contains_texture(render_batch *batch, unsigned int texture_ID) {
        if (batch->texture_count < 8) {
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
