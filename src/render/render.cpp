/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "render/render.h"

#include <cstdio>

#include "core/game_object.h"
#include "render/render_batch.h"
#include "core/mouse_listener.h"
#include "util/collision_math.h"

using namespace std;

namespace Render {

    static RenderBatch::render_batch batches[100];
    static RenderBatch::render_batch *sorted_batches[100];
    static int batch_count = 0;
    static const int max_batch_size = 1000;

    /**
     * Render elements to the screen.
     */
    void render() {
        // Render each batch one at a time, in order of z-index
        for (int i = 0; i < batch_count; i++) {
            RenderBatch::render(sorted_batches[i]);
        }
    }

    /**
     * Add a game object to the next available render batch.
     * @param obj Game object to be added
     */
    void add_game_object(GameObject::game_object *obj) {
        for (int i = 0; i < batch_count; i++) {
            // If there is enough space to add the object and its texture, and the z-index matches, add it to the first available batch
            if (batches[i].has_room && (RenderBatch::contains_texture(&batches[i], obj->sprite.texture_ID) || obj->sprite.is_null) && obj->z_index == batches[i].z_index) {
                printf("Added object %s to batch %d\n",obj->name.c_str(), i);
                RenderBatch::add_game_object(&batches[i], obj);
                return;
            }
        }
        // If no batch has space for the game object or if there are no available texture slots, or if there is a new z-index, create a new batch
        printf("Added object %s to new batch %d\n",obj->name.c_str(), batch_count);
        //printf("Created new batch with z-index %d\n", obj->z_index);
        RenderBatch::render_batch batch {};
        RenderBatch::init(&batch, max_batch_size, obj->z_index);
        batches[batch_count] = batch;
        sorted_batches[batch_count] = &batches[batch_count];
        RenderBatch::add_game_object(&batches[batch_count], obj);
        batch_count++;

        // Sort the list of batches by z-index
        for (int i = 0; i < batch_count - 1; i++) {
            for (int j = 0; j < batch_count - 1 - i; j++) {
                if (sorted_batches[j]->z_index > sorted_batches[j + 1]->z_index) {
                    RenderBatch::render_batch *temp;
                    temp = sorted_batches[j];
                    sorted_batches[j] = sorted_batches[j + 1];
                    sorted_batches[j + 1] = temp;
                }
            }
        }
    }

    /**
     * Highlight the game object under the mouse cursor.
     */
    void highlight_game_object(GameObject::game_object **highlighted_obj) {
        double xPos = Mouse::get_worldX();
        double yPos = Mouse::get_worldY();

        // Loop through each batch, starting at the highest z-index
        for (int i = batch_count - 1; i >= 0; i--) {
            // Loop through the game objects, starting with the most recently rendered
            for (int j = sorted_batches[i]->game_object_count - 1; j >= 0; j--) {
                // Check if the mouse position is within the object's bounding box
                GameObject::game_object *obj = sorted_batches[i]->game_object_list[j];
                if (xPos > obj->position.x && xPos < obj->position.x + obj->scale.x
                    && yPos > obj->position.y && yPos < obj->position.y + obj->scale.y
                    && obj->pickable && obj->visible) {
                    *highlighted_obj = obj;
                    return;
                }
            }
        }
        // If no object is found, set the highlighted object to null
        *highlighted_obj = nullptr;
    }

    /**
     * Highlight multiple game objects within a selection box.
     * @param highlighted_objects Highlighted game objects list
     * @param highlighted_count Number of highlighted objects
     * @param selection_pos Starting position of selection
     * @param selection_scale Size of selection
     */
    void highlight_game_objects(GameObject::game_object **highlighted_objects, int *highlighted_count, glm::vec2 selection_pos, glm::vec2 selection_scale) {
        *highlighted_count = 0;
        // Loop through each batch
        for (int i = batch_count - 1; i >= 0; i--) {
            // Loop through the game objects
            for (int j = sorted_batches[i]->game_object_count - 1; j >= 0; j--) {
                // Check if the object's bounding box intersects with the selection box
                GameObject::game_object *obj = sorted_batches[i]->game_object_list[j];
                if (obj->pickable && obj->visible &&
                    Math::line_segment_collision(obj->position.x, obj->position.x + obj->scale.x, selection_pos.x, selection_pos.x + selection_scale.x) &&
                    Math::line_segment_collision(obj->position.y, obj->position.y + obj->scale.y, selection_pos.y, selection_pos.y + selection_scale.y)) {
                    GameObject::set_highlighted(obj, true);
                    highlighted_objects[*highlighted_count] = obj;
                    *highlighted_count = *highlighted_count + 1;
                } else {
                    GameObject::set_highlighted(obj, false);
                }
            }
        }
    }

    /**
     * Clear the render batches (used when deserializing)
     */
    void clear_render_batches() {
        batch_count = 0;
    }
}
