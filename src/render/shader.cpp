/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

#include "render/shader.h"

#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;

namespace Shader {

    static GLuint program_ID;

    /**
     * Create and compile the shader program
     */
    void create_program() {

        string vertex_shader_source, fragment_shader_source;
        GLuint vertex_shader, fragment_shader;
        GLint success, len;
        GLsizei length;
        GLchar description[1000];

        // Read shader source file
        string path = "../assets/shaders/default.glsl";
        ifstream input_file;
        input_file.open(path);
        if (!input_file.is_open()) {
            cerr << "Could not open the file - '"
                 << path << "'" << endl;
            exit(EXIT_FAILURE);
        }
        string shader_source = string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

        // Use regex to split the shader into vertex and fragment
        std::regex rgx("#type( )+([a-zA-Z])+");
        std::sregex_token_iterator iter(shader_source.begin(),shader_source.end(),rgx,-1);
        iter++;
        vertex_shader_source = string(*iter);
        iter++;
        fragment_shader_source = string(*iter);

        const char *vertex_shader_text = vertex_shader_source.c_str();
        const char *fragment_shader_text = fragment_shader_source.c_str();

        // Compile vertex shader
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &len);
            glGetShaderInfoLog(vertex_shader, len, &length, description);
            printf("Error: Vertex shader compilation failed\n");
            printf("%s", description);
            exit(EXIT_FAILURE);
        }

        // Compile fragment shader
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &len);
            glGetShaderInfoLog(fragment_shader, len, &length, description);
            printf("Error: Fragment shader compilation failed\n");
            printf("%s", description);
            exit(EXIT_FAILURE);
        }

        // Link shaders
        program_ID = glCreateProgram();
        glAttachShader(program_ID, vertex_shader);
        glAttachShader(program_ID, fragment_shader);
        glLinkProgram(program_ID);
        glGetProgramiv(program_ID, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            glGetProgramiv(program_ID, GL_INFO_LOG_LENGTH, &len);
            glGetProgramInfoLog(program_ID, len, &length, description);
            printf("Error: Linking of shaders failed\n");
            printf("%s", description);
            exit(EXIT_FAILURE);
        }
    }

    void use_program() {
        glUseProgram(program_ID);
    }

    void detach() {
        glUseProgram(0);
    }

    void upload_mat4(const char *varName, glm::mat4 matrix) {
        GLint var_location = glGetUniformLocation(program_ID, varName);
        glUniformMatrix4fv(var_location, 1, false, glm::value_ptr(matrix));
    }

    void upload_texture(const char *varName, int slot) {
        GLint var_location = glGetUniformLocation(program_ID, varName);
        glUniform1i(var_location, slot);
    }

    void upload_textures(const char *varName, int *slots) {
        GLint var_location = glGetUniformLocation(program_ID, varName);
        glUniform1iv(var_location, 8, slots);
    }
}
