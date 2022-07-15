/**
 * @author Charlie Davidson
 * Created on 7/3/22.
 */

#include "sprite_manager.h"

#include "editor/interface/drag_objects.h"

namespace Editor {
    namespace SpriteManager {

        void init(Editor::SpriteManager::sprite_manager *obj, glm::vec2 position, glm::vec2 scale, int z_index, Sprite::sprite *sprite) {
            // Essential data
            obj->position = position;
            obj->scale = scale;
            obj->z_index = z_index;
            obj->sprite = *sprite;
            obj->color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
            obj->saturation = 0.0f;

            // Editor information
            obj->is_dirty = true;
            obj->out_color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
            obj->pickable = true;
            obj->visible = true;
            obj->highlighted = false;
            obj->selected = false;
            obj->new_z_index = z_index;
        }

        void set_sprite(Editor::SpriteManager::sprite_manager *obj, Sprite::sprite *spr) {
            obj->sprite = *spr;
            obj->is_dirty = true;
        }

        void set_position(Editor::SpriteManager::sprite_manager *obj, glm::vec2 position) {
            obj->position = position;
            obj->is_dirty = true;
        }

        void set_scale(Editor::SpriteManager::sprite_manager *obj, glm::vec2 scale) {
            obj->scale = scale;
            obj->is_dirty = true;
        }

        void set_color(Editor::SpriteManager::sprite_manager *obj, glm::vec4 color) {
            obj->color = color;
            obj->out_color = color;
            obj->is_dirty = true;
            update_color(obj);
        }

        void set_saturation(Editor::SpriteManager::sprite_manager *obj, float saturation) {
            obj->saturation = saturation;
            obj->is_dirty = true;
        }

        void set_visible(Editor::SpriteManager::sprite_manager *obj, bool visible) {
            obj->visible = visible;
            update_color(obj);
        }

        void set_highlighted(Editor::SpriteManager::sprite_manager *obj, bool highlighted) {
            obj->highlighted = highlighted;
            update_color(obj);
        }

        void set_selected(Editor::SpriteManager::sprite_manager *obj, bool selected) {
            obj->selected = selected;
            update_color(obj);
        }

        /**
         * Update the output color of a sprite
         * @param obj Game object
         */
        void update_color(Editor::SpriteManager::sprite_manager *obj) {
            glm::vec3 new_color{};

            if (obj->selected && Drag::is_invalid_placement()) {
                obj->saturation = 0.4f;
                new_color = glm::vec3{1.0f, 0.3f, 0.3f};
            } else {
                if (obj->selected) {
                    if (obj->highlighted) {
                        obj->saturation = 0.2f;
                        new_color = glm::vec3{0.3f, 0.3f, 1.0f};
                    } else {
                        obj->saturation = 0.2f;
                        new_color = glm::vec3{0.0f, 0.0f, 1.0f};
                    }
                } else {
                    if (obj->highlighted) {
                        obj->saturation = 0.3f;
                        new_color = glm::vec3{1.0f, 1.0f, 1.0f};
                    } else {
                        obj->saturation = 0.0f;
                        new_color = glm::vec3{0.0f, 0.0f, 0.0f};
                    }
                }
            }
            if (!obj->sprite.is_null) {
                obj->out_color.x = new_color.x;
                obj->out_color.y = new_color.y;
                obj->out_color.z = new_color.z;
            } else {
                obj->out_color.x = obj->color.x;
                obj->out_color.y = obj->color.y;
                obj->out_color.z = obj->color.z;
            }

            if (obj->visible) {
                obj->out_color.w = obj->color.w;
            } else {
                obj->saturation = 1.0f;
                obj->out_color.w = 0.0f;
            }
            obj->is_dirty = true;
        }
    }
}
