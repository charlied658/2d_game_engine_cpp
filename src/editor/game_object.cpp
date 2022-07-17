/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#include "game_object.h"

namespace Editor {
    namespace GameObject {

        static int id_counter = 0;
        static int transient_id_counter = 0;

        void init(Editor::GameObject::game_object *obj) {
            obj->id = id_counter;
            id_counter++;
            obj->has_sprite_manager = false;
            obj->has_physics_manager = false;
            obj->has_behavior_manager = false;
            obj->transient = false;
            obj->dead = false;
        }

        void init_transient(Editor::GameObject::game_object *obj) {
            obj->id = transient_id_counter;
            transient_id_counter++;
            obj->has_sprite_manager = false;
            obj->has_physics_manager = false;
            obj->has_behavior_manager = false;
            obj->transient = true;
            obj->dead = false;
        }

        void init_sprite_manager(Editor::GameObject::game_object *obj, Editor::SpriteManager::sprite_manager *spr_manager) {
            obj->spr_manager = spr_manager;
            obj->spr_manager->game_object = obj;
            obj->spr_manager->id = obj->id;
            obj->has_sprite_manager = true;
        }

        void init_physics_manager(Editor::GameObject::game_object *obj, Editor::PhysicsManager::physics_manager *py_manager) {
            obj->py_manager = py_manager;
            obj->py_manager->game_object = obj;
            obj->py_manager->id = obj->id;
            obj->has_physics_manager = true;
        }

        void init_behavior_manager(Editor::GameObject::game_object *obj, Editor::BehaviorManager::behavior_manager *bh_manager) {
            obj->bh_manager = bh_manager;
            obj->bh_manager->game_object = obj;
            obj->bh_manager->id = obj->id;
            obj->has_behavior_manager = true;
        }

        /**
         * Set the ID counter. Used when deserializing.
         * @param id
         */
        void set_id(int id) {
            id_counter = id;
        }
    }
}
