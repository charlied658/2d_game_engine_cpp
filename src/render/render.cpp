/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "render/render.h"

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <cstdio>

#include "core/game_object.h"
#include "render/texture.h"
#include "render/render_batch.h"

using namespace std;

struct vertex {
    float x, y, z; // Position
    float r, g, b; // Color
    float u, v;    // Texture coordinates
    float t;       // Texture ID
};

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
    static RenderBatch::render_batch *batches[100];
    static int batch_count = 0;
    static const int max_batch_size = 1000;

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

        // Generate Element Buffer Object (EBO)
        glGenBuffers(1, &eboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

        // Enable vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * 6));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * 8));
        glEnableVertexAttribArray(3);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        // Generate textures
        textureID = Texture::get_texture("../assets/images/testImage.png")->textureID;
        textureID2 = Texture::get_texture("../assets/images/testImage2.png")->textureID;

        // Generate game objects
        GameObject::game_object obj1 {"obj1", 2.0f, 1.0f, 1.0f, 1.0f, textureID};
        GameObject::game_object obj2 {"obj2", 3.0f, 1.0f, 1.0f, 1.0f, textureID2};

        // Add game objects to the scene
        Render::add_game_object(&obj1);
        Render::add_game_object(&obj2);


        for (int i = 0; i < batch_count; i++) {
            //printf("count: %d\n", batches[i]->game_object_count);
            //RenderBatch::render(batches[i]);
            printf("count: %d\n", batches[i]->game_object_count);
        }
    }

    /**
     * Draw elements to the screen.
     */
    void render() {

        for (int i = 0; i < batch_count; i++) {
            //printf("count: %d\n", batches[i]->game_object_count);
            RenderBatch::render(batches[i]);
            //printf("count: %d\n", batches[i]->game_object_count);
        }

        //printf("count1: %d\n", batches[0]->game_object_count);
    }

    void add_game_object(GameObject::game_object *obj) {
        GameObject::add_game_object(obj);
        for (int i = 0; i < batch_count; i++) {
            if (batches[i]->has_room) {
                printf("Added object to batch %d\n", i);
                RenderBatch::add_game_object(batches[i], obj);
                return;
            }
        }
        // If no batch has space for the game object, create a new batch
        printf("Created new batch at index %d\n", batch_count);
        RenderBatch::render_batch batch {};
        RenderBatch::init(&batch, max_batch_size);
        batches[batch_count] = &batch;
        RenderBatch::add_game_object(batches[batch_count], obj);
        batch_count++;
    }
}
