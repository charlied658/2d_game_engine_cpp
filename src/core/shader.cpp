/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "shader.h"

#include "glm/gtc/type_ptr.hpp"
#include <cstdio>
#include <sstream>

#include "util/properties.h"

namespace Shader {

    static shader shader_list[100];
    static int shader_count;
    static GLuint current_program_ID;

    /**
     * Get a shader from the shader list. If it is not in the list, add it to the list.
     * @param shader Shader object
     * @param filepath Filepath of shader
     */
    void get_shader(unsigned int *shader, const std::string& vertex_filepath, const std::string& fragment_filepath) {
        for (int i = 0; i < shader_count; i++) {
            if (shader_list[i].vertex_filepath == vertex_filepath && shader_list[i].fragment_filepath == fragment_filepath) {
                *shader = shader_list[i].program_ID;
                return;
            }
        }
        // If no match is found, create a shader and add it to the shader list.
        shader_list[shader_count] = Shader::shader {};
        shader_list[shader_count].vertex_filepath = vertex_filepath;
        shader_list[shader_count].fragment_filepath = fragment_filepath;
        shader_list[shader_count].program_ID = create_program(vertex_filepath, fragment_filepath);
        *shader = shader_list[shader_count].program_ID;
        shader_count++;
    }

    /**
     * Create and compile the shader program.
     */
    static unsigned int create_program(const std::string& vertex_filepath, const std::string& fragment_filepath) {
        std::string vertex_shader_source, fragment_shader_source;
        GLuint vertex_shader, fragment_shader, program_ID;
        GLint success, len;
        GLsizei length;
        GLchar description[1000];

        // Read shader source files
        Shader::read_source(&vertex_shader_source, vertex_filepath);
        Shader::read_source(&fragment_shader_source, fragment_filepath);
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
        return program_ID;
    }

    /**
     * Set the given shader program to be in use.
     * @param program_ID Program ID
     */
    void use_program(unsigned int program_ID) {
        glUseProgram(program_ID);
        current_program_ID = program_ID;
    }

    /**
     * Read the source of a shader file.
     * @param source Source text
     * @param filepath Filepath of shader
     */
    void read_source(std::string *source, const std::string& filepath) {
        std::string absolute_filepath = PROJECT_PATH + filepath;
        std::ifstream input_file;
        input_file.open(absolute_filepath);
        if (!input_file.is_open()) {
            printf("Could not open file '%s'\n", absolute_filepath.c_str());
            exit(EXIT_FAILURE);
        }
        std::stringstream buffer;
        buffer << input_file.rdbuf();
        *source = buffer.str();
    }

    void upload_mat4(const char *varName, glm::mat4 matrix) {
        GLint var_location = glGetUniformLocation(current_program_ID, varName);
        glUniformMatrix4fv(var_location, 1, false, glm::value_ptr(matrix));
    }

    void upload_textures(const char *varName, int *slots) {
        GLint var_location = glGetUniformLocation(current_program_ID, varName);
        glUniform1iv(var_location, 8, slots);
    }
}
