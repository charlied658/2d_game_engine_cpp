/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#pragma once

#include "line.h"

#include "glad/glad.h"

namespace Game {
    namespace LineBatch {

        struct line_batch {
            Game::Line::line **line_list;
            int line_count;
            int max_batch_size;
            bool has_room;
            float *vertex_data;
            int *element_data;
            GLuint vaoID, vboID, eboID;
        };

        void init(Game::LineBatch::line_batch *batch, int max_batch_size);

        void render(Game::LineBatch::line_batch *batch);

        void add_line(Game::LineBatch::line_batch *batch, Line::line *line);

        static void generate_vertex_data(Game::LineBatch::line_batch *batch, int index);

        static void generate_element_indices(Game::LineBatch::line_batch *batch);

    }
}
