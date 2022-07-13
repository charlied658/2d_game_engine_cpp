/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "sprite_renderer.h"

#include "core/sprite_manager.h"
#include "sprite_batch.h"
#include "core/mouse_listener.h"
#include "editor/collision/collision_math.h"

namespace SpriteRenderer {

    static SpriteBatch::sprite_batch batches[100];
    static SpriteBatch::sprite_batch *sorted_batches[100];
    static int batch_count = 0;
    static const int max_batch_size = 1000;

    /**
     * Render sprites to the screen.
     */
    void render() {
        // Render each batch one at a time, in order of z-index
        for (int i = 0; i < batch_count; i++) {
            SpriteBatch::render(sorted_batches[i]);
        }
    }

    /**
     * Add a sprite to the next available sprite batch.
     * @param obj Sprite
     */
    void add_sprite(SpriteManager::sprite_manager *obj) {
        for (int i = 0; i < batch_count; i++) {
            // If there is enough space to add the sprite and its texture, and the z-index matches, add it to the first available batch
            if (batches[i].has_room && (SpriteBatch::contains_texture(&batches[i], obj->sprite.texture_ID) || obj->sprite.is_null) && obj->z_index == batches[i].z_index) {
                SpriteBatch::add_sprite(&batches[i], obj);
                return;
            }
        }
        // If no batch has space for the sprite or if there are no available texture slots, or if there is a new z-index, create a new batch
        SpriteBatch::sprite_batch batch {};
        SpriteBatch::init(&batch, max_batch_size, obj->z_index);
        batches[batch_count] = batch;
        sorted_batches[batch_count] = &batches[batch_count];
        SpriteBatch::add_sprite(&batches[batch_count], obj);
        batch_count++;

        // Sort the list of batches by z-index
        for (int i = 0; i < batch_count - 1; i++) {
            for (int j = 0; j < batch_count - 1 - i; j++) {
                if (sorted_batches[j]->z_index > sorted_batches[j + 1]->z_index) {
                    SpriteBatch::sprite_batch *temp;
                    temp = sorted_batches[j];
                    sorted_batches[j] = sorted_batches[j + 1];
                    sorted_batches[j + 1] = temp;
                }
            }
        }
    }

    /**
     * Remove a sprite from its render batch.
     * @param obj Sprite
     */
    void remove_sprite(SpriteManager::sprite_manager *obj) {
        for (int i = 0; i < batch_count; i++) {
            if (batches[i].z_index == obj->z_index) {
                if (SpriteBatch::remove_sprite(&batches[i], obj)) {
                    return;
                }
            }
        }
    }

    /**
     * Removes a sprite from its batch and re-adds it to a new one. Useful for resetting z-index.
     * @param obj Sprite
     */
    void re_add_sprite(SpriteManager::sprite_manager *obj) {
        remove_sprite(obj);
        obj->z_index = obj->new_z_index;
        add_sprite(obj);
    }

    /**
     * Highlight the sprite under the mouse cursor.
     */
    void highlight_sprite(SpriteManager::sprite_manager **highlighted_obj) {
        double xPos = Mouse::get_worldX();
        double yPos = Mouse::get_worldY();

        // Loop through each batch, starting at the highest z-index
        for (int i = batch_count - 1; i >= 0; i--) {
            // Loop through the sprites, starting with the most recently rendered
            for (int j = sorted_batches[i]->sprite_count - 1; j >= 0; j--) {
                // Check if the mouse position is within the object's bounding box
                SpriteManager::sprite_manager *obj = sorted_batches[i]->sprite_list[j];
                if (xPos > obj->position.x && xPos < obj->position.x + obj->scale.x
                    && yPos > obj->position.y && yPos < obj->position.y + obj->scale.y
                    && obj->pickable && obj->visible) {
                    SpriteManager::set_highlighted(obj, true);
                    *highlighted_obj = obj;
                    return;
                }
            }
        }
        // If no sprite is found, set the highlighted object to null
        *highlighted_obj = nullptr;
    }

    /**
     * Highlight multiple sprites within a selection box.
     * @param highlighted_objects Highlighted objects list
     * @param highlighted_count Number of highlighted objects
     * @param selection_pos Starting position of selection
     * @param selection_scale Size of selection
     */
    void highlight_sprites(SpriteManager::sprite_manager **highlighted_objects, int *highlighted_count, SpriteManager::sprite_manager *selection_box) {
        *highlighted_count = 0;
        glm::vec2 selection_pos = selection_box->position;
        glm::vec2 selection_scale = selection_box->scale;
        // Loop through each batch
        for (int i = batch_count - 1; i >= 0; i--) {
            // Loop through the sprites
            for (int j = sorted_batches[i]->sprite_count - 1; j >= 0; j--) {
                // Check if the sprite's bounding box intersects with the selection box
                SpriteManager::sprite_manager *obj = sorted_batches[i]->sprite_list[j];
                if (*highlighted_count < 1000 && obj->pickable && obj->visible &&
                    Math::line_segment_collision(obj->position.x, obj->position.x + obj->scale.x, selection_pos.x, selection_pos.x + selection_scale.x) &&
                    Math::line_segment_collision(obj->position.y, obj->position.y + obj->scale.y, selection_pos.y, selection_pos.y + selection_scale.y)) {
                    SpriteManager::set_highlighted(obj, true);
                    highlighted_objects[*highlighted_count] = obj;
                    *highlighted_count = *highlighted_count + 1;
                } else {
                    SpriteManager::set_highlighted(obj, false);
                }
            }
        }
    }

    /**
     * Clear the render batches (used when deserializing)
     */
    void clear_render_batches() {
        // Free memory
        for (int i = 0; i < batch_count; i++) {
            delete batches[i].sprite_list;
        }
        batch_count = 0;
    }
}
