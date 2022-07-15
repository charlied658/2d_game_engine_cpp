/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#pragma once

#include "glm/vec2.hpp"

#include "editor/game_object.h"
#include "physics/box_collider.h"
#include "physics/circle_collider.h"

namespace Editor {
    namespace PhysicsManager {

        struct physics_manager {
            int id;
            Editor::GameObject::game_object *game_object;

            glm::vec2 position;
            glm::vec2 scale;
            glm::vec2 velocity;
            glm::vec2 acceleration;
            BoxCollider::box_collider box_collider;
            CircleCollider::circle_collider circle_collider;
            bool has_box_collider;
            bool has_circle_collider;
        };

        void init(Editor::PhysicsManager::physics_manager *py_manager);

        void init_box_collider(Editor::PhysicsManager::physics_manager *py_manager);

        void init_circle_collider(Editor::PhysicsManager::physics_manager *py_manager);

    }
}
