/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#include "line_renderer.h"
#include "line_batch.h"

namespace LineRenderer {

    static LineBatch::line_batch batches[100];
    static int batch_count = 0;
    static const int max_batch_size = 1000;

    /**
     * Render elements to the screen.
     */
    void render() {
        // Render each batch one at a time
        for (int i = 0; i < batch_count; i++) {
            LineBatch::render(&batches[i]);
        }
    }

    /**
     * Add a line to the next available line batch.
     * @param line Line to be added
     */
    void add_line(Line::line *line) {
        for (int i = 0; i < batch_count; i++) {
            // If there is enough space to add the object, add it to the first available batch
            if (batches[i].has_room) {
                LineBatch::add_line(&batches[i], line);
                return;
            }
        }
        // If no batch has space for the line, create a new batch
        LineBatch::line_batch batch {};
        LineBatch::init(&batch, max_batch_size);
        batches[batch_count] = batch;
        LineBatch::add_line(&batches[batch_count], line);
        batch_count++;
    }
}
