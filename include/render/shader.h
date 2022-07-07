/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#pragma once

#include <glad/glad.h>

#include <glm/mat4x4.hpp> // glm::mat4

#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace Shader {
    void create_program();

    void use_program();

    void detach();

    void upload_mat4(const char *varName, glm::mat4);

    void upload_texture(const char *varName, int slot);

    void upload_textures(const char *varName, int *slots);
}