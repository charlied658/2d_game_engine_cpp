//
// Created by Charlie Davidson on 6/27/22.
//

#pragma once

#include <glm/glm.hpp>

namespace Shader {
    void create_program();

    void use_program();

    void detach();

    void upload_mat4(const char* varName, glm::mat4 matrix);
}