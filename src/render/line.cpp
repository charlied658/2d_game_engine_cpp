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
     * Calculate the coordinates of the quad representing a line with given width.
     * @param line Line reference
     * TODO: This breaks with lines with semi-flat slopes. Use a different method
     */
    void calculate_coordinates(Line::line *line) {
        float slope = (line->end_point.y - line->start_point.y) / (line->end_point.x - line->start_point.x);
        float dx, dy;
        if (slope == 0) {
            dx = 0;
            dy = line->width / 2.0f;
        } else {
            float perpendicular_slope = -1.0f / slope;
            dx = line->width / (2.0f * sqrt(1 + perpendicular_slope));
            dy = dx * perpendicular_slope;
        }
        line->coordinates[0] = glm::vec2 {line->end_point.x - dx, line->end_point.y - dy};
        line->coordinates[1] = glm::vec2 {line->end_point.x + dx, line->end_point.y + dy};
        line->coordinates[2] = glm::vec2 {line->start_point.x + dx, line->start_point.y + dy};
        line->coordinates[3] = glm::vec2 {line->start_point.x - dx, line->start_point.y - dy};
    }
}
