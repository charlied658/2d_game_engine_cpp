/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#pragma once

#include "line.h"

#include "glad/glad.h"

namespace Editor {
    namespace LineBatch {

        struct line_batch {
            Editor::Line::line **line_list;
            int line_count;
            int max_batch_size;
            bool has_room;
            float *vertex_data;
            int *element_data;
            GLuint vaoID, vboID, eboID;
        };

        void init(Editor::LineBatch::line_batch *batch, int max_batch_size);

        void render(Editor::LineBatch::line_batch *batch);

        void add_line(Editor::LineBatch::line_batch *batch, Editor::Line::line *line);

        static void generate_vertex_data(Editor::LineBatch::line_batch *batch, int index);

        static void generate_element_indices(Editor::LineBatch::line_batch *batch);

    }
}
