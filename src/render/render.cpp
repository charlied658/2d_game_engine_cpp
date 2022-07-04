/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "render/render.h"

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <cstdio>

#include "core/game_object.h"
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
    static int vertex_count;
    static float vertex_data[10000];
    static int element_data[10000];

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
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        // Generate Element Buffer Object (EBO)
        glGenBuffers(1, &eboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_indices), element_indices, GL_STATIC_DRAW);

        // Enable vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * 6));
        glEnableVertexAttribArray(2);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Generate textures
        textureID = Texture::get_texture("../assets/images/testImage.png")->textureID;
        textureID2 = Texture::get_texture("../assets/images/testImage2.png")->textureID;

        // Generate game objects
        GameObject::game_object obj1 {"obj1", 2.0f, 1.0f, 1.0f, 1.0f, textureID};
        GameObject::game_object obj2 {"obj2", 3.0f, 1.0f, 1.0f, 1.0f, textureID2};

        //GameObject::add_game_object(&obj1);
        //GameObject::add_game_object(&obj2);

        // Generate 100 game objects to test vertex generation
        for (int i = 0; i < 100; i++) {
            float xPos = 2.0f + (float) (i % 10) / 10.0f;
            float yPos = 2.0f - (float) (i / 10) / 10.0f;
            GameObject::game_object obj {"obj", xPos, yPos, 0.1f, 0.1f, textureID};
            GameObject::add_game_object(&obj);
        }

        // Generate vertex data from list of game objects
        generate_vertex_data();

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
        glBindTexture(GL_TEXTURE_2D, textureID2);

        // Draw elements
        glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, nullptr);
    }

    /**
     * Generate vertex data from the list of game objects.
     */
    static void generate_vertex_data() {
        GameObject::list game_object_list = *GameObject::get_game_object_list();
        vertex_count = game_object_list.count * 6;

        int offset = 0;
        int element_offset = 0;
        float xAdd, yAdd;
        for (int i = 0; i < game_object_list.count; i++) {
            xAdd = 0.0f;
            yAdd = 0.0f;
            for (int j = 0; j < 4; j++) {
                if (j == 1) {
                    xAdd = 1.0f;
                } else if (j == 2) {
                    yAdd = 1.0f;
                } else if (j == 3){
                    xAdd = 0.0f;
                }

                // Position
                vertex_data[offset + 0] = (xAdd * game_object_list.game_objects[i].x_scale) + game_object_list.game_objects[i].x_pos;
                vertex_data[offset + 1] = (yAdd * game_object_list.game_objects[i].y_scale) + game_object_list.game_objects[i].y_pos;
                vertex_data[offset + 2] = 0.0f;

                // Color
                vertex_data[offset + 3] = 1.0f;
                vertex_data[offset + 4] = 1.0f;
                vertex_data[offset + 5] = 1.0f;

                // Texture Coordinates
                vertex_data[offset + 6] = xAdd;
                vertex_data[offset + 7] = yAdd;

                offset += 8;
            }

            // Update element indices
            element_data[i * 6 + 0] = element_offset + 0;
            element_data[i * 6 + 1] = element_offset + 1;
            element_data[i * 6 + 2] = element_offset + 2;
            element_data[i * 6 + 3] = element_offset + 0;
            element_data[i * 6 + 4] = element_offset + 2;
            element_data[i * 6 + 5] = element_offset + 3;

            element_offset += 4;
        }

        // Bind data to buffers
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_data), element_data, GL_STATIC_DRAW);
    }
}
