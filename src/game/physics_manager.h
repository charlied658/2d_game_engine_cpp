/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#pragma once

#include "glm/vec2.hpp"

namespace Game {
    namespace PhysicsManager {

        struct physics_manager {
            glm::vec2 position;
            glm::vec2 scale;
            glm::vec2 velocity;
            glm::vec2 acceleration;
            bool box_collider;
            bool circle_collider;
        };

        void init(Game::PhysicsManager::physics_manager *py_manager);

        void init_box_collider(Game::PhysicsManager::physics_manager *py_manager);

        void init_circle_collider(Game::PhysicsManager::physics_manager *py_manager);

    }
}

