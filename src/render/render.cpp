/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "render/render.h"

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <cstdio>

#include "render/shader.h"
#include "core/camera.h"
#include "render/texture.h"

struct vertex {
    float x, y, z; // Position
    float r, g, b; // Color
    float u, v;    // Texture coordinates
};

vertex vertices[] = {
        {2.0f, 1.0f, 0.0f, 1.0f,0.0f,0.0f, 0.0f, 0.0f},
        {3.0f, 1.0f, 0.0f, 0.0f,1.0f,0.0f, 1.0f, 0.0f},
        {3.0f,  2.0f, 0.0f, 0.0f,0.0f,1.0f, 1.0f, 1.0f},
        {2.0f,  2.0f, 0.0f, 0.0f,1.0f,1.0f, 0.0f, 1.0f},
        {3.0f, 1.0f, 0.0f, 1.0f,0.0f,0.0f, 0.0f, 0.0f},
        {4.0f, 1.0f, 0.0f, 0.0f,1.0f,0.0f, 1.0f, 0.0f},
        {4.0f,  2.0f, 0.0f, 0.0f,0.0f,1.0f, 1.0f, 1.0f},
        {3.0f,  2.0f, 0.0f, 0.0f,1.0f,1.0f, 0.0f, 1.0f}
};

int element_indices[] = {0,1,2,0,2,3,
                         4,5,6,4,6,7};

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
    static unsigned int textureID ,textureID2;

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
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * 6));
        glEnableVertexAttribArray(2);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Generate textures
        textureID = Texture::get_texture("../assets/images/testImage.png")->textureID;
        textureID2 = Texture::get_texture("../assets/images/testImage2.png")->textureID;
    }

    /**
     * Draw elements to the screen.
     */
    void draw() {

        // Upload view and projection matrices to the shader program
        Shader::upload_mat4("view", Camera::get_view());
        Shader::upload_mat4("projection", Camera::get_projection());

        // Bind texture 1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Draw first texture
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Draw second texture
        glBindTexture(GL_TEXTURE_2D, textureID2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) (sizeof(unsigned int) * 6));
    }
}
