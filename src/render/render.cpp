//
// Created by Charlie Davidson on 6/27/22.
//

#include "render/render.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include "render/shader.h"
#include "render/camera.h"
#include "core/key_listener.h"
#include "util/vecmath.h"

struct vertex {
    float x, y, z;
    float r, g, b;
};

vertex vertices[] = {
        {-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f},
        {0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f},
        {0.5f,  0.5f, 0.0f, 0.0f,0.0f,1.0f},
        {-0.5f,  0.5f, 0.0f, 0.0f,1.0f,1.0f}
};

int element_indices[] = {0,1,2, 0,2,3};

/*
 * ========= Diagram of one Quad: ==========
 *
 *               3-----------2
 *               |           |
 *               |           |
 *               |           |
 *               0-----------1
 *
 *  Triangles:
 *      0 -> 1 -> 2
 *      0 -> 2 -> 3
 *
 *  NOTE: Vertices should be given in COUNTER-CLOCKWISE
 *  order according to OpenGL
 *
 */

namespace Render {

    static GLuint vaoID, vboID, eboID;

    /**
     * Initialize OpenGL buffers to be drawn to the window
     */
    void init() {
        // Generate Vertex Array Object (VAO)
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);

        // Generate Vertex Buffer Object (VBO)
        glGenBuffers(1, &vboID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        // Generate Element Buffer Object (EBO)
        glGenBuffers(1, &eboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_indices), element_indices, GL_STATIC_DRAW);

        // Enable vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        Camera::adjust_projection();
    }

    /**
     * Draw elements to the screen
     * @param dt Delta time
     */
    void draw(double dt) {
        // Rotate the square when user presses left/right (temp feature)
        if (Key::get_key_pressed(GLFW_KEY_LEFT) && !Key::get_key_pressed(GLFW_KEY_RIGHT)) {
            for (int i = 0; i < 4; i++) {
                vector2 point = {vertices[i].x, vertices[i].y};
                vector2 result = Math::rotate(point, vector2{0,0}, dt * 0.5f);
                vertices[i].x = result.x;
                vertices[i].y = result.y;
            }
        }
        if (Key::get_key_pressed(GLFW_KEY_RIGHT) && !Key::get_key_pressed(GLFW_KEY_LEFT)) {
            for (int i = 0; i < 4; i++) {
                vector2 point = {vertices[i].x, vertices[i].y};
                vector2 result = Math::rotate(point, vector2{0,0}, dt * -0.5f);
                vertices[i].x = result.x;
                vertices[i].y = result.y;
            }
        }

        // Update the buffer data
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        Shader::upload_mat4("uProjection", Camera::get_projection());
        Shader::upload_mat4("uView", Camera::get_view());

        // Draw elements to the screen
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}
