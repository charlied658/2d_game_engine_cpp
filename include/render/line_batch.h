/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#pragma once

#include "render/line.h"

#include <glad/glad.h>

namespace LineBatch {

    struct line_batch {
        Line::line **line_list;
        int line_count;
        int max_batch_size;
        bool has_room;
        float *vertex_data;
        int *element_data;
        GLuint vaoID, vboID, eboID;
    };

    void init(LineBatch::line_batch *batch, int max_batch_size);

    void render(LineBatch::line_batch *batch);

    void add_line(LineBatch::line_batch *batch, Line::line *line);

    static void generate_vertex_data(LineBatch::line_batch *batch, int index);

    static void generate_element_indices(LineBatch::line_batch *batch);

}
