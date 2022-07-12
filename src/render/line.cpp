/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#include "render/line.h"

#include <cmath>

namespace Line {

    /**
     * Initialize a line.
     * @param line Line reference
     * @param start_point Starting point of line
     * @param end_point Ending point of line
     * @param width Width of line
     * @param color Color of line
     */
    void init(Line::line *line, glm::vec2 start_point, glm::vec2 end_point, float width, glm::vec4 color) {
        line->start_point = start_point;
        line->end_point = end_point;
        line->width = width;
        line->color = color;
        calculate_coordinates(line);
    }

    /**
     * Set the width of a line.
     * @param line Line reference
     * @param width Line width
     */
    void set_width(Line::line *line, float width) {
        line->width = width;
        calculate_coordinates(line);
    }

    /**
     * Calculate the coordinates of the quad representing a line with given width.
     * @param line Line reference
     */
    void calculate_coordinates(Line::line *line) {
        float dx, dy;
        float dx_p, dy_p;
        float h;
        dx = line->end_point.x - line->start_point.x;
        dy = line->end_point.y - line->start_point.y;
        h = hypotf(dx, dy);
        dx_p = (dy * line->width) / (2.0f * h);
        dy_p = (dx * line->width) / (2.0f * h);
        line->coordinates[0] = glm::vec2 {line->end_point.x + dx_p, line->end_point.y - dy_p};
        line->coordinates[1] = glm::vec2 {line->end_point.x - dx_p, line->end_point.y + dy_p};
        line->coordinates[2] = glm::vec2 {line->start_point.x - dx_p, line->start_point.y + dy_p};
        line->coordinates[3] = glm::vec2 {line->start_point.x + dx_p, line->start_point.y - dy_p};
    }
}
