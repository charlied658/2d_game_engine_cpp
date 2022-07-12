/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#include "editor/grid_lines.h"

#include "render/line.h"
#include "render/line_renderer.h"

namespace GridLines {

    static Line::line *line_list;

    /**
     * Initialize grid lines.
     */
    void init() {
        line_list = new Line::line[100];
        for (int i = 0; i < 50; i++) {
            Line::init(&line_list[i], glm::vec2 {(float) (i - 16) * 0.25f, 4.25f},
                       glm::vec2 {(float) (i - 16) * 0.25f, -3.0f}, 0.01f, glm::vec4 {0.0f, 0.0f, 0.5f, 0.2f});
            LineRenderer::add_line(&line_list[i]);
        }
        for (int i = 0; i < 30; i++) {
            Line::init(&line_list[i + 50], glm::vec2 {-4.0f, (float) (i - 12) * 0.25f},
                       glm::vec2 {8.25f, (float) (i - 12) * 0.25f}, 0.01f, glm::vec4 {0.0f, 0.0f, 0.5f, 0.2f});
            LineRenderer::add_line(&line_list[i + 50]);
        }
    }
}
