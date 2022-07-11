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

using namespace std;

namespace Shader {

    struct shader {
        unsigned int program_ID;
        string vertex_filepath;
        string fragment_filepath;
    };

    void get_shader(unsigned int *shader, const string& vertex_filepath, const string& fragment_filepath);

    static unsigned int create_program(const string& vertex_filepath, const string& fragment_filepath);

    void use_program(unsigned int program_ID);

    void read_source(string *source, const string& filepath);

    void upload_mat4(const char *varName, glm::mat4);

    void upload_textures(const char *varName, int *slots);
}