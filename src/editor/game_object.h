/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#pragma once

#include "editor/sprite_manager.h"
#include "editor/behavior_manager.h"
#include "editor/physics_manager.h"

namespace Editor {

    namespace SpriteManager {
        struct sprite_manager;
    }

    namespace BehaviorManager {
        struct behavior_manager;
    }

    namespace PhysicsManager {
        struct physics_manager;
    }

    namespace GameObject {

        struct game_object {
            int id;
            std::string name;
            bool transient;

            Editor::SpriteManager::sprite_manager *spr_manager;
            Editor::PhysicsManager::physics_manager *py_manager;
            Editor::BehaviorManager::behavior_manager *bh_manager;
            bool has_sprite_manager;
            bool has_physics_manager;
            bool has_behavior_manager;

            bool dead;

            template<class Archive>
            void serialize(Archive &archive) {
                archive(id, name, transient, has_sprite_manager, has_physics_manager, has_behavior_manager);
            }
        };

        void init(Editor::GameObject::game_object *obj);

        void init_transient(Editor::GameObject::game_object *obj);

        void init_sprite_manager(Editor::GameObject::game_object *obj, Editor::SpriteManager::sprite_manager *spr_manager);

        void init_physics_manager(Editor::GameObject::game_object *obj, Editor::PhysicsManager::physics_manager *py_manager);

        void init_behavior_manager(Editor::GameObject::game_object *obj, Editor::BehaviorManager::behavior_manager *bh_manager);

    }
}
