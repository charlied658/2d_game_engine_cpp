//
// Created by Charlie Davidson on 6/28/22.
//

#pragma once

#include <glm/glm.hpp>

namespace Camera {

    void init();

    void move_camera(glm::vec2 dv);

    void scale_camera(float scale);

    glm::mat4 get_view();

    glm::mat4 get_projection();
}
