/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#include "game_object.h"

namespace Editor {
    namespace GameObject {

        static int id_counter = 0;

        void init(Editor::GameObject::game_object *obj) {
            obj->id = id_counter;
            id_counter++;
            obj->has_sprite_manager = false;
            obj->has_physics_manager = false;
            obj->has_behavior_manager = false;
            obj->transient = false;
            obj->dead = false;
        }

        void init_sprite_manager(Editor::GameObject::game_object *obj, Editor::SpriteManager::sprite_manager *spr_manager) {
            obj->spr_manager = spr_manager;
            obj->spr_manager->game_object = obj;
            obj->spr_manager->id = obj->id;
            obj->has_sprite_manager = true;
        }
    }
}
