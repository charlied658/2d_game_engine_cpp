//
// Created by Charlie Davidson on 6/27/22.
//

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <cstdio>

#include "render/shader.h"

namespace Shader {

    static const char* vertex_shader_text =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aColor;\n"
            "out vec3 fColor;"
            "void main() {\n"
            "    fColor = aColor;\n"
            "    gl_Position = vec4(aPos, 1.0);\n"
            "}";

    static const char* fragment_shader_text =
            "#version 330 core\n"
            "in vec3 fColor;\n"
            "out vec4 color;\n"
            "void main() {\n"
            "    color = vec4(fColor, 1.0f);\n"
            "}";

    /**
     * Create and compile the shader program
     * @param program Program index
     */
    void create_program(GLuint* program) {

        GLuint vertex_shader, fragment_shader;
        GLint success, len;
        GLsizei length;
        GLchar description[1000];

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
        }

        // Link shaders
        *program = glCreateProgram();
        glAttachShader(*program, vertex_shader);
        glAttachShader(*program, fragment_shader);
        glLinkProgram(*program);
        glGetProgramiv(*program, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &len);
            glGetProgramInfoLog(*program, len, &length, description);
            printf("Error: Linking of shaders failed\n");
            printf("%s", description);
        }
    }
}
