//
// Created by Charlie Davidson on 6/27/22.
//

#include "render/render.h"

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include "render/shader.h"
#include "render/camera.h"

struct vertex {
    float x, y, z;
    float r, g, b;
};

vertex vertices[] = {
        {0.0f, 0.0f, 0.0f, 1.0f,0.0f,0.0f},
        {1.0f, 0.0f, 0.0f, 0.0f,1.0f,0.0f},
        {1.0f,  1.0f, 0.0f, 0.0f,0.0f,1.0f},
        {0.0f,  1.0f, 0.0f, 0.0f,1.0f,1.0f}
};

int element_indices[] = {0,1,2,0,2,3};

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
     * Initialize OpenGL buffers to be drawn to the window.
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
    }

    /**
     * Draw elements to the screen.
     */
    void draw() {

        // Upload view and projection matrices to the shader program
        Shader::upload_mat4("view", Camera::get_view());
        Shader::upload_mat4("projection", Camera::get_projection());

        // Draw elements to the screen
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}
