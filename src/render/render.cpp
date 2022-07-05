/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "render/render.h"

#include <cstdio>

#include "core/game_object.h"
#include "render/texture.h"
#include "render/render_batch.h"

using namespace std;

namespace Render {

    static unsigned int textureID ,textureID2;
    static RenderBatch::render_batch batches[100];
    static int batch_count = 0;
    static const int max_batch_size = 100;

    /**
     * Initialize OpenGL buffers to be drawn to the window.
     */
    void init() {

        // Generate OpenGL buffers
        RenderBatch::generate_buffers();

        // Generate textures
        textureID = Texture::get_texture("../assets/images/testImage.png")->textureID;
        textureID2 = Texture::get_texture("../assets/images/testImage2.png")->textureID;

        // Generate game objects
        GameObject::game_object obj1 {"obj1", 2.0f, 1.0f, 1.0f, 1.0f, textureID};
        GameObject::game_object obj2 {"obj2", 3.0f, 1.0f, 1.0f, 1.0f, textureID2};

        // Add 100 game objects to the scene (to test render batching)
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                GameObject::game_object obj {};
                obj.name = "obj";
                obj.x_pos = 4.0f + (float) i / 10.0f;
                obj.y_pos = 1.0f + (float) j / 10.0f;
                obj.x_scale = 0.1f;
                obj.y_scale = 0.1f;
                obj.textureID = textureID;
                Render::add_game_object(&obj);
            }
        }

        // Add game objects to the scene
        Render::add_game_object(&obj1);
        Render::add_game_object(&obj2);
    }

    /**
     * Draw elements to the screen.
     */
    void render() {
        // Render each batch one at a time
        for (int i = 0; i < batch_count; i++) {
            RenderBatch::render(&batches[i]);
        }
    }

    /**
     * Add a game object to the next available render batch.
     * @param obj Game object to be added
     */
    static void add_game_object(GameObject::game_object *obj) {
        GameObject::add_game_object(obj);
        for (int i = 0; i < batch_count; i++) {
            if (batches[i].has_room) {
                RenderBatch::add_game_object(&batches[i], obj);
                return;
            }
        }
        // If no batch has space for the game object, create a new batch
        printf("Created new batch at index %d\n", batch_count);
        RenderBatch::render_batch batch {};
        RenderBatch::init(&batch, max_batch_size);
        batches[batch_count] = batch;
        RenderBatch::add_game_object(&batches[batch_count], obj);
        batch_count++;
    }
}
