/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#include "editor/grid_lines.h"

#include "core/camera.h"
#include "core/render/line.h"
#include "core/render/line_renderer.h"

namespace GridLines {

    static Line::line *line_list_vertical;
    static Line::line *line_list_horizontal;
    static int line_count_vertical;
    static int line_count_horizontal;
    static float line_width = 0.01f;
    static glm::vec4 color;

    static const float world_width = 6.0f;
    static const float world_height = 3.0f;
    static const float block_width = 0.25f;
    static const float block_height = 0.25f;

    /**
     * Initialize grid lines.
     */
    void init() {
        line_list_vertical = new Line::line[1000];
        line_list_horizontal = new Line::line[1000];
        line_count_vertical = 0;
        line_count_horizontal = 0;
        color = glm::vec4 {0.0f,0.0f,5.0f,0.2f};
    }

    /**
     * Reset the grid lines to be invisible.
     */
    void reset() {
        for (int i = 0; i < line_count_vertical; i++) {
            line_list_vertical[i].color.w = 0.0f;
        }
        for (int i = 0; i < line_count_horizontal; i++) {
            line_list_horizontal[i].color.w = 0.0f;
        }
    }

    /**
     * Update the grid lines.
     */
    void update() {
        float zoom = Camera::getZoom();
        glm::vec2 position = Camera::get_position();
        int index;

        // Reset grid lines
        reset();

        // Calculate X offset
        float paddingX = world_width * (1 - (1 / zoom)) / 2;
        float startX = -position.x + paddingX;
        float endX = -position.x + world_width - paddingX;

        // Calculate Y offset
        float paddingY = world_height * (1 - (1 / zoom)) / 2;
        float startY = -position.y + paddingY;
        float endY = -position.y + world_height - paddingY;

        // Update vertical lines
        float lineX = startX - fmod(startX, block_width);
        index = 0;
        while (lineX < endX) {
            Line::init(&line_list_vertical[index], glm::vec2(lineX, startY), glm::vec2(lineX, endY), line_width, color);
            if (index >= line_count_vertical) {
                if (line_count_vertical < 1000) {
                    line_count_vertical++;
                    LineRenderer::add_line(&line_list_vertical[index]);
                } else {
                    break;
                }
            }
            lineX += block_width;
            index++;
        }

        // Update horizontal lines
        float lineY = startY - fmod(startY, block_height);
        index = 0;
        while (lineY < endY) {
            Line::init(&line_list_horizontal[index], glm::vec2(startX, lineY), glm::vec2(endX, lineY), line_width, color);
            if (index >= line_count_horizontal) {
                if (line_count_horizontal < 1000) {
                    line_count_horizontal++;
                    LineRenderer::add_line(&line_list_horizontal[index]);
                } else {
                    break;
                }
            }
            lineY += block_width;
            index++;
        }
    }
}
