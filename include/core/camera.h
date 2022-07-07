/**
 * @author Charlie Davidson
 * Created on 6/28/22.
 */

#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "core/key_listener.h"

namespace Camera {

    void init();

    void update(double dt);

    void move_camera(glm::vec2 dv);

    void scale_camera(float scale);

    glm::mat4 get_view();

    glm::mat4 get_projection();

    glm::mat4 get_inverseView();

    glm::mat4 get_inverseProjection();

}
