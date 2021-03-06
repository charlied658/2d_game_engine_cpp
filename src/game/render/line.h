/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#pragma once

#include "glm/vec2.hpp" // glm::vec2
#include "glm/vec4.hpp" // glm::vec4

namespace Game {
    namespace Line {

        struct line {
            glm::vec2 start_point;
            glm::vec2 end_point;
            float width;
            glm::vec4 color;
            glm::vec2 coordinates[4];
        };

        void init(Game::Line::line *line, glm::vec2 start_point, glm::vec2 end_point, float width, glm::vec4 color);

        void set_width(Game::Line::line *line, float width);

        void calculate_coordinates(Game::Line::line *line);

    }
}
