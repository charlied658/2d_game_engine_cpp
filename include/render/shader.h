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
#include <string>

namespace Shader {

    struct shader {
        unsigned int program_ID;
        std::string vertex_filepath;
        std::string fragment_filepath;
    };

    void get_shader(unsigned int *shader, const std::string& vertex_filepath, const std::string& fragment_filepath);

    static unsigned int create_program(const std::string& vertex_filepath, const std::string& fragment_filepath);

    void use_program(unsigned int program_ID);

    void read_source(std::string *source, const std::string& filepath);

    void upload_mat4(const char *varName, glm::mat4);

    void upload_textures(const char *varName, int *slots);
}