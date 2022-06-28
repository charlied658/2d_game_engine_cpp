//
// Created by Charlie Davidson on 6/28/22.
//

#include "render/camera.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

namespace Camera {
    static glm::vec2 position;
    static glm::mat4 projection, view;

    void move_camera(glm::vec2 dv) {
        position.x += dv.x;
        position.y += dv.y;
    }

    void adjust_projection() {
        projection = glm::ortho(-1,1,-1,1);
    }

    glm::mat4 get_view() {
        glm::vec3 camera_front = glm::vec3(0.0f,0.0f,-1.0f);
        glm::vec3 camera_up = glm::vec3(0.0f,1.0f,0.0f);
        view = glm::lookAt(glm::vec3(position.x, position.y, 20.0f), camera_front, camera_up);
        return view;
    }

    glm::mat4 get_projection() {
        return projection;
    }
}
