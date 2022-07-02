//
// Created by Charlie Davidson on 6/28/22.
//

#pragma once

#include <glm/glm.hpp>
#include "util/vecmath.h"

namespace Camera {

    void init();

    void move_camera(glm::vec2 dv);

    void scale_camera(float scale);

    Math::mat4 get_view();

    Math::mat4 get_projection();
}
