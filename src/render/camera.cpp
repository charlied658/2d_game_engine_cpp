//
// Created by Charlie Davidson on 6/28/22.
//

#include "render/camera.h"

#include "core/window.h"

#include <glm/glm.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::transform, glm::perspective

namespace Camera {
    static glm::vec2 position, rotation;
    static float zoom = 1.0f;

    void init() {
        position = {0.0f,0.0f};
        rotation = {1.0f, 0.5f};
        zoom = 1.0f;
    }

    void move_camera(glm::vec2 dv) {
        position += dv / zoom;
    }

    void scale_camera(float scale) {
        zoom *= scale;
    }

    void rotate_camera(glm::vec2 dv) {
        rotation += dv;
    }

    /**
     * Calculate the model matrix.
     * @return Model matrix
     */
    glm::mat4 get_model() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
        return model;
    }

    /**
     * Calculate the view matrix.
     * @return View matrix
     */
    glm::mat4 get_view() {
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(position.x, position.y, -3.0f));
        return view;
    }

    /**
     * Calculate the projection matrix.
     * @return Projection matrix
     */
    glm::mat4 get_projection() {
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), Window::get_aspect_ratio(), 0.1f, 100.0f);
        return projection;
    }
}
