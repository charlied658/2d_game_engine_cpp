//
// Created by Charlie Davidson on 6/28/22.
//

#include "render/camera.h"
#include "util/vecmath.h"

#include "core/window.h"

#include <glm/glm.hpp>

namespace Camera {
    static glm::vec2 position;
    static Math::mat4 view_matrix;
    static float zoom = 1.0f;

    void init() {
        position = {0.0f,0.0f};
        zoom = 1.0f;
    }

    void move_camera(glm::vec2 dv) {
        position += dv / zoom;
    }

    void scale_camera(float scale) {
        zoom *= scale;
    }

    /**
     * Calculate the view matrix.
     * @return View matrix
     */
    Math::mat4 get_view() {
        view_matrix = Math::identity();
        view_matrix.m[12] = position.x;
        view_matrix.m[13] = position.y;

        Math::mat4 scale = Math::identity();
        float ratio = Window::get_aspect_ratio();
        if (ratio < 1) {
            scale.m[0] = zoom;
            scale.m[5] = zoom * Window::get_aspect_ratio();
        } else {
            scale.m[0] = zoom / Window::get_aspect_ratio();
            scale.m[5] = zoom;
        }

        Math::mat4 result = Math::mul(scale, view_matrix);
        return result;
    }

    Math::mat4 get_projection() {
        return Math::identity();
    }
}
