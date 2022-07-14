/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#include "physics_manager.h"

namespace Editor {
    namespace PhysicsManager {

        void init(Editor::PhysicsManager::physics_manager *py_manager) {
            py_manager->position = glm::vec2{};
            py_manager->scale = glm::vec2{};
            py_manager->velocity = glm::vec2{};
            py_manager->acceleration = glm::vec2{};
            py_manager->has_box_collider = false;
            py_manager->has_circle_collider = false;
        }

        void init_box_collider(Editor::PhysicsManager::physics_manager *py_manager) {
            init(py_manager);
            py_manager->has_box_collider = true;
        }

        void init_circle_collider(Editor::PhysicsManager::physics_manager *py_manager) {
            init(py_manager);
            py_manager->has_circle_collider = true;
        }

    }
}
