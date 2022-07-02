//
// Created by Charlie Davidson on 6/28/22.
//

#include "render/camera.h"

#include <glm/glm.hpp> // glm::vec2, glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::transform, glm::ortho

namespace Camera {
    static glm::vec2 position;
    static float zoom = 1.0f;
    static const float screen_width = 6.0f;
    static const float screen_height = 3.0f;

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
    glm::mat4 get_view() {
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(position.x, position.y, 0.0f));
        return view;
    }

    /**
     * Calculate the projection matrix.
     * @return Projection matrix
     */
    glm::mat4 get_projection() {
        glm::mat4 projection;

        float xOffset = screen_width * (1.0f - 1.0f / zoom) / 2;
        float yOffset = screen_height * (1.0f - 1.0f / zoom) / 2;

        projection = glm::ortho(xOffset, screen_width / zoom + xOffset, yOffset, screen_height / zoom + yOffset);
        return projection;
    }
}
