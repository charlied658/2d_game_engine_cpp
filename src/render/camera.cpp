//
// Created by Charlie Davidson on 6/28/22.
//

#include "render/camera.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <cstdio>

namespace Camera {
    static glm::vec2 position;
    static glm::mat4 projection, view;
    static float matrix[] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

    void move_camera(glm::vec2 dv) {
        //position.x += dv.x;
        //position.y += dv.y;
        matrix[12] += dv.x;
        matrix[13] += dv.y;
        //printf("%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n\n", matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5], matrix[6], matrix[7], matrix[8], matrix[9], matrix[10], matrix[11], matrix[12], matrix[13], matrix[14], matrix[15]);
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

    float *get_projection() {
        return matrix;
    }
}
