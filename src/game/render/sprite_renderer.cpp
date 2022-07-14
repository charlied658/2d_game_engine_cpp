/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "sprite_renderer.h"

#include "game/sprite_manager.h"
#include "sprite_batch.h"
#include "core/mouse_listener.h"

namespace Game {
    namespace SpriteRenderer {

        static Game::SpriteBatch::sprite_batch batches[100];
        static Game::SpriteBatch::sprite_batch *sorted_batches[100];
        static int batch_count = 0;
        static const int max_batch_size = 1000;

        /**
         * Render sprites to the screen.
         */
        void render() {
            // Render each batch one at a time, in order of z-index
            for (int i = 0; i < batch_count; i++) {
                Game::SpriteBatch::render(sorted_batches[i]);
            }
        }

        /**
         * Add a sprite to the next available sprite batch.
         * @param obj Sprite
         */
        void add_sprite(Game::SpriteManager::sprite_manager *obj) {
            for (int i = 0; i < batch_count; i++) {
                // If there is enough space to add the sprite and its texture, and the z-index matches, add it to the first available batch
                if (batches[i].has_room &&
                    (Game::SpriteBatch::contains_texture(&batches[i], obj->sprite.texture_ID) || obj->sprite.is_null) &&
                    obj->z_index == batches[i].z_index) {
                    Game::SpriteBatch::add_sprite(&batches[i], obj);
                    return;
                }
            }
            // If no batch has space for the sprite or if there are no available texture slots, or if there is a new z-index, create a new batch
            Game::SpriteBatch::sprite_batch batch{};
            Game::SpriteBatch::init(&batch, max_batch_size, obj->z_index);
            batches[batch_count] = batch;
            sorted_batches[batch_count] = &batches[batch_count];
            Game::SpriteBatch::add_sprite(&batches[batch_count], obj);
            batch_count++;

            // Sort the list of batches by z-index
            for (int i = 0; i < batch_count - 1; i++) {
                for (int j = 0; j < batch_count - 1 - i; j++) {
                    if (sorted_batches[j]->z_index > sorted_batches[j + 1]->z_index) {
                        Game::SpriteBatch::sprite_batch *temp;
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
        void remove_sprite(Game::SpriteManager::sprite_manager *obj) {
            for (int i = 0; i < batch_count; i++) {
                if (batches[i].z_index == obj->z_index) {
                    if (Game::SpriteBatch::remove_sprite(&batches[i], obj)) {
                        return;
                    }
                }
            }
        }

        /**
         * Removes a sprite from its batch and re-adds it to a new one. Useful for resetting z-index.
         * @param obj Sprite
         */
        void re_add_sprite(Game::SpriteManager::sprite_manager *obj) {
            remove_sprite(obj);
            obj->z_index = obj->new_z_index;
            add_sprite(obj);
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
}
