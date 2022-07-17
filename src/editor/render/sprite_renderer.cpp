/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "sprite_renderer.h"

#include "editor/sprite_manager.h"
#include "sprite_batch.h"
#include "core/mouse_listener.h"
#include "editor/collision/collision_math.h"
#include "imgui/imgui.h"

namespace Editor {
    namespace SpriteRenderer {

        static Editor::SpriteBatch::sprite_batch batches[100];
        static Editor::SpriteBatch::sprite_batch *sorted_batches[100];
        static int batch_count = 0;
        static const int max_batch_size = 1000;

        static Editor::SpriteBatch::sprite_batch transient_batches[100];
        static int transient_batch_count = 0;
        static int total_batch_count = 0;

        /**
         * Render sprites to the screen.
         */
        void render() {
            // Render each batch one at a time, in order of z-index
            //printf("Starting rendering\n");
            for (int i = 0; i < total_batch_count; i++) {
                //printf("Rendering batch %d\n",sorted_batches[i]->z_index);
                Editor::SpriteBatch::render(sorted_batches[i]);
            }
        }

        /**
         * Add a sprite to the next available sprite batch.
         * @param obj Sprite
         */
        void add_sprite(Editor::SpriteManager::sprite_manager *obj) {
            //printf("Added sprite %s with z-index %d and id %d\n", obj->game_object->name.c_str(), obj->z_index, obj->id);
            for (int i = 0; i < batch_count; i++) {
                // If there is enough space to add the sprite and its texture, and the z-index matches, add it to the first available batch
                if (batches[i].has_room &&
                    (Editor::SpriteBatch::contains_texture(&batches[i], obj->sprite.texture_ID) || obj->sprite.is_null) &&
                    obj->z_index == batches[i].z_index) {
                    Editor::SpriteBatch::add_sprite(&batches[i], obj);
                    return;
                }
            }
            // If no batch has space for the sprite or if there are no available texture slots, or if there is a new z-index, create a new batch
            Editor::SpriteBatch::sprite_batch batch{};
            Editor::SpriteBatch::init(&batch, max_batch_size, obj->z_index);
            batches[batch_count] = batch;
            sorted_batches[total_batch_count] = &batches[batch_count];
            Editor::SpriteBatch::add_sprite(&batches[batch_count], obj);
            batch_count++;
            total_batch_count = batch_count + transient_batch_count;

            // Sort the list of batches by z-index
            sort_batches();
        }

        /**
         * Remove a sprite from its render batch.
         * @param obj Sprite
         */
        void remove_sprite(Editor::SpriteManager::sprite_manager *obj) {
            for (int i = 0; i < batch_count; i++) {
                if (batches[i].z_index == obj->z_index) {
                    if (Editor::SpriteBatch::remove_sprite(&batches[i], obj)) {
                        return;
                    }
                }
            }
        }

        /**
         * Removes a sprite from its batch and re-adds it to a new one. Useful for resetting z-index.
         * @param obj Sprite
         */
        void re_add_sprite(Editor::SpriteManager::sprite_manager *obj) {
            remove_sprite(obj);
            obj->z_index = obj->new_z_index;
            add_sprite(obj);
        }

        void add_transient_sprite(Editor::SpriteManager::sprite_manager *obj) {
            for (int i = 0; i < transient_batch_count; i++) {
                // If there is enough space to add the sprite and its texture, and the z-index matches, add it to the first available batch
                if (transient_batches[i].has_room &&
                    (Editor::SpriteBatch::contains_texture(&transient_batches[i], obj->sprite.texture_ID) || obj->sprite.is_null) &&
                    obj->z_index == transient_batches[i].z_index) {
                    //printf("Added transient sprite %s with z-index %d\n", obj->game_object->name.c_str(), obj->z_index);
                    Editor::SpriteBatch::add_sprite(&transient_batches[i], obj);
                    return;
                }
            }
            //printf("Created new transient batch with z-index %d\n", obj->z_index);
            // If no batch has space for the sprite or if there are no available texture slots, or if there is a new z-index, create a new batch
            Editor::SpriteBatch::sprite_batch transient_batch{};
            Editor::SpriteBatch::init(&transient_batch, max_batch_size, obj->z_index);
            transient_batches[transient_batch_count] = transient_batch;
            sorted_batches[total_batch_count] = &transient_batches[transient_batch_count];
            Editor::SpriteBatch::add_sprite(&transient_batches[transient_batch_count], obj);
            transient_batch_count++;
            total_batch_count = batch_count + transient_batch_count;

            // Sort the list of batches by z-index
            sort_batches();
        }

        /**
         * Highlight the sprite under the mouse cursor.
         */
        void highlight_sprite(Editor::GameObject::game_object **highlighted_obj) {
            double xPos = Mouse::get_worldX();
            double yPos = Mouse::get_worldY();

            // Loop through each batch, starting at the highest z-index
            for (int i = total_batch_count - 1; i >= 0; i--) {
                // Loop through the sprites, starting with the most recently rendered
                for (int j = sorted_batches[i]->sprite_count - 1; j >= 0; j--) {
                    // Check if the mouse position is within the object's bounding box
                    Editor::SpriteManager::sprite_manager *obj = sorted_batches[i]->sprite_list[j];
                    if (xPos > obj->position.x && xPos < obj->position.x + obj->scale.x
                        && yPos > obj->position.y && yPos < obj->position.y + obj->scale.y
                        && obj->pickable && obj->visible) {
                        Editor::SpriteManager::set_highlighted(obj, true);
                        *highlighted_obj = obj->game_object;
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
        void highlight_sprites(Editor::GameObject::game_object **highlighted_objects, int *highlighted_count,
                               Editor::GameObject::game_object *selection_box) {
            *highlighted_count = 0;
            glm::vec2 selection_pos = selection_box->spr_manager->position;
            glm::vec2 selection_scale = selection_box->spr_manager->scale;
            // Loop through each batch
            for (int i = total_batch_count - 1; i >= 0; i--) {
                // Loop through the sprites
                for (int j = sorted_batches[i]->sprite_count - 1; j >= 0; j--) {
                    // Check if the sprite's bounding box intersects with the selection box
                    Editor::SpriteManager::sprite_manager *obj = sorted_batches[i]->sprite_list[j];
                    if (*highlighted_count < 1000 && obj->pickable && obj->visible &&
                        Math::line_segment_collision(obj->position.x, obj->position.x + obj->scale.x, selection_pos.x,
                                                     selection_pos.x + selection_scale.x) &&
                        Math::line_segment_collision(obj->position.y, obj->position.y + obj->scale.y, selection_pos.y,
                                                     selection_pos.y + selection_scale.y)) {
                        Editor::SpriteManager::set_highlighted(obj, true);
                        //printf("Highlighted object %d\n", j);
                        //printf("Name: %s\n", obj->game_object->spr_manager->name.c_str());
                        highlighted_objects[*highlighted_count] = obj->game_object;
                        *highlighted_count = *highlighted_count + 1;
                    } else {
                        SpriteManager::set_highlighted(obj, false);
                    }
                }
            }
        }

        /**
         * Sort the render batches in order of z-index.
         */
        void sort_batches() {
            for (int i = 0; i < total_batch_count - 1; i++) {
                for (int j = 0; j < total_batch_count - 1 - i; j++) {
                    if (sorted_batches[j]->z_index > sorted_batches[j + 1]->z_index) {
                        Editor::SpriteBatch::sprite_batch *temp;
                        temp = sorted_batches[j];
                        sorted_batches[j] = sorted_batches[j + 1];
                        sorted_batches[j + 1] = temp;
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
            total_batch_count = 0;
            for (int i = 0; i < transient_batch_count; i++) {
                sorted_batches[i] = &transient_batches[i];
                total_batch_count++;
            }
            sort_batches();
        }

        void imgui() {
            for (int i = 0; i < total_batch_count; i++) {
                ImGui::Text("Batch %d: Z-index %d, Count %d", i, sorted_batches[i]->z_index, sorted_batches[i]->sprite_count);
            }
        }
    }
}
