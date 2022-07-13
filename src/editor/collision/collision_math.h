/**
 * @author Charlie Davidson 
 * Created on 7/8/22.
 */

#pragma once

#include "glm/vec2.hpp" //glm::vec2

namespace Math {

    bool line_segment_collision(float a_x1, float a_x2, float b_x1, float b_x2);

    float f_mod(float a, float b);

    int i_mod(int a, int b);

    glm::vec2 grid_position(glm::vec2 position);

}
