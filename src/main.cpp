#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <cstdio>

const char* vertex_shader_text =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 fColor;"
        "void main() {\n"
        "    fColor = aColor;\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "}";

const char* fragment_shader_text =
        "#version 330 core\n"
        "in vec3 fColor;\n"
        "out vec4 color;\n"
        "void main() {\n"
        "    color = vec4(fColor, 1.0f);\n"
        "}";

float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,
        0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,
        0.0f,  0.5f, 0.0f, 0.0f,0.0f,1.0f
};

static void error_callback(int error, const char* description) {
    printf("Error: %s\n", description);
}

int main()
{
    GLFWwindow* window;
    GLuint vertex_shader, fragment_shader, program;
    GLuint vaoID, vboID;
    GLint success, len;
    GLsizei length;
    GLchar *description;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "My Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6,nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

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

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        glGetProgramInfoLog(program, len, &length, description);
        printf("Error: Linking of shaders failed\n");
        printf("%s", description);
    }

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}