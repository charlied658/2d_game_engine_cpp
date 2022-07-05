/**
 * @author Charlie Davidson 
 * Created on 7/4/22.
 */

#pragma once

#include "core/game_object.h"

namespace RenderBatch {

    struct render_batch {
        GameObject::game_object **game_object_list;
        int game_object_count;
        int max_batch_size;
        bool has_room;
        float *vertex_data;
        int *element_data;
        unsigned int *texture_list;
        int texture_count;
    };

    void init(render_batch *batch, int max_batch_size);

    void render(render_batch *batch);

    void add_game_object(render_batch *batch, GameObject::game_object *obj);

    static void generate_vertex_data(render_batch *batch, int index);

    static void generate_element_indices(render_batch *batch);

    static int get_texture_slot(render_batch *batch, unsigned int texture_ID);

    void generate_buffers();
}