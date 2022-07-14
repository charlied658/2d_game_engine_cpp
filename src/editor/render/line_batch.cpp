/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#include "line_batch.h"

#include "core/shader.h"
#include "core/camera.h"

namespace Editor {
    namespace LineBatch {

        // Vertex information
        static const int position_size = 2;
        static const int color_size = 4;

        static const int position_offset = 0;
        static const int color_offset = position_size;

        static const int vertex_size = position_size + color_size;
        static const int vertex_size_bytes = sizeof(float) * vertex_size;

        /**
         * Create and initialize a line batch.
         * @param batch Line batch reference
         * @param max_batch_size Maximum allowed number of lines
         */
        void init(Editor::LineBatch::line_batch *batch, int max_batch_size) {
            batch->max_batch_size = max_batch_size;
            batch->line_list = new Editor::Line::line *[max_batch_size];
            batch->line_count = 0;
            batch->has_room = true;
            batch->vertex_data = new float[batch->max_batch_size * 4 * vertex_size];
            batch->element_data = new int[batch->max_batch_size * 6];

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

            // Enable alpha blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        /**
         * Render the lines in a line batch.
         * @param batch Line batch reference
         */
        void render(Editor::LineBatch::line_batch *batch) {

            // Update line vertex information
            for (int i = 0; i < batch->line_count; i++) {
                generate_vertex_data(batch, i);
            }

            // Rebuffer vertex data
            glBindBuffer(GL_ARRAY_BUFFER, batch->vboID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, (long) sizeof(float) * (batch->line_count) * 4 * vertex_size,
                            batch->vertex_data);

            // Upload view and projection matrices to the shader program
            Shader::upload_mat4("view", Camera::get_view());
            Shader::upload_mat4("projection", Camera::get_projection());

            // Draw elements
            glBindVertexArray(batch->vaoID);
            glDrawElements(GL_TRIANGLES, 6 * batch->line_count, GL_UNSIGNED_INT, nullptr);
        }

        /**
         * Add a line to the batch.
         * @param batch Line batch reference
         * @param line Line to be added
         */
        void add_line(Editor::LineBatch::line_batch *batch, Editor::Line::line *line) {
            batch->line_list[batch->line_count] = line;

            // Generate vertex data for the newly added line
            generate_vertex_data(batch, batch->line_count);

            // Increment line count
            batch->line_count++;
            if (batch->line_count >= batch->max_batch_size) {
                batch->has_room = false;
            }
        }

        /**
         * Generate vertex data for the given line.
         * @param batch Line batch reference
         * @param index Index to begin adding vertex data
         */
        static void generate_vertex_data(Editor::LineBatch::line_batch *batch, int index) {
            int offset = index * vertex_size * 4;
            for (int j = 0; j < 4; j++) {

                // ============== Upload vertex information
                // Position
                batch->vertex_data[offset + 0] = batch->line_list[index]->coordinates[j].x;
                batch->vertex_data[offset + 1] = batch->line_list[index]->coordinates[j].y;

                // Color
                batch->vertex_data[offset + 2] = batch->line_list[index]->color.x;
                batch->vertex_data[offset + 3] = batch->line_list[index]->color.y;
                batch->vertex_data[offset + 4] = batch->line_list[index]->color.z;
                batch->vertex_data[offset + 5] = batch->line_list[index]->color.w;

                offset += vertex_size;
            }
        }

        /**
         * Generate element indices.
         * @param batch Line batch reference
         */
        static void generate_element_indices(Editor::LineBatch::line_batch *batch) {
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
    }
}
