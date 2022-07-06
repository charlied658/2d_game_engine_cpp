/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "render/render.h"

#include <cstdio>

#include "core/game_object.h"
#include "render/render_batch.h"

using namespace std;

namespace Render {

    static RenderBatch::render_batch batches[100];
    static int batch_count = 0;
    static const int max_batch_size = 1000;

    /**
     * Render elements to the screen.
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
    void add_game_object(GameObject::game_object *obj) {
        GameObject::add_game_object(obj);
        for (int i = 0; i < batch_count; i++) {
            // If there is enough space to add the object and its texture, add it to the first available batch
            if (batches[i].has_room && RenderBatch::contains_texture(&batches[i], obj->textureID)) {
                RenderBatch::add_game_object(&batches[i], obj);
                return;
            }
        }
        // If no batch has space for the game object or if there are no available texture slots, create a new batch
        RenderBatch::render_batch batch {};
        RenderBatch::init(&batch, max_batch_size);
        batches[batch_count] = batch;
        RenderBatch::add_game_object(&batches[batch_count], obj);
        batch_count++;
    }
}
