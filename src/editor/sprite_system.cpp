/**
 * @author Charlie Davidson 
 * Created on 7/14/22.
 */

#include "sprite_system.h"

namespace Editor {
    namespace SpriteSystem {

        static Editor::SpriteManager::sprite_manager *sprite_manager_list;
        static int sprite_manager_count;
        static Editor::SpriteManager::sprite_manager *transient_sprite_manager_list;
        static int transient_sprite_manager_count;

        void init() {
            sprite_manager_list = new Editor::SpriteManager::sprite_manager[10000];
            sprite_manager_count = 0;
            transient_sprite_manager_list = new Editor::SpriteManager::sprite_manager[10000];
            transient_sprite_manager_count = 0;
        }

        void reload() {
            sprite_manager_count = 0;
        }

        void init_sprite_manager(Editor::SpriteManager::sprite_manager **spr_manager) {
            if (sprite_manager_count < 10000) {
                //printf("Added sprite manager %d\n", sprite_manager_count);
                *spr_manager = &sprite_manager_list[sprite_manager_count];
                sprite_manager_count++;
            }
        }

        void init_transient_sprite_manager(Editor::SpriteManager::sprite_manager **spr_manager) {
            if (transient_sprite_manager_count < 10000) {
                //printf("Added sprite manager %d\n", sprite_manager_count);
                *spr_manager = &transient_sprite_manager_list[transient_sprite_manager_count];
                transient_sprite_manager_count++;
            }
        }

        void remove_sprite_manager(Editor::SpriteManager::sprite_manager *spr_manager) {
            bool found = false;
            for (int i = 0; i < sprite_manager_count; i++) {
                if (&sprite_manager_list[i] == spr_manager) {
                    found = true;
                }
                if (found && i < sprite_manager_count - 1) {
                    sprite_manager_list[i] = sprite_manager_list[i + 1];
                    sprite_manager_list[i].game_object->spr_manager = &sprite_manager_list[i];
                }
            }
            if (found) {
                sprite_manager_count--;
            }
        }

        void get_sprite_manager_list(Editor::SpriteManager::sprite_manager **objects, int *object_count) {
            *objects = sprite_manager_list;
            *object_count = sprite_manager_count;
        }

        void get_transient_sprite_manager_list(Editor::SpriteManager::sprite_manager **objects, int *object_count) {
            *objects = transient_sprite_manager_list;
            *object_count = transient_sprite_manager_count;
        }
    }
}