/**
 * @author Charlie Davidson 
 * Created on 7/14/22.
 */

#include "physics_system.h"

namespace PhysicsSystem {

    static Editor::PhysicsManager::physics_manager *physics_manager_list;
    static int physics_manager_count;

    void init() {
        physics_manager_list = new Editor::PhysicsManager::physics_manager[1000];
        physics_manager_count = 0;
    }

    void reload() {
        physics_manager_count = 0;
    }

    void add_physics_manager(Editor::PhysicsManager::physics_manager *py_manager) {
        physics_manager_list[physics_manager_count] = *py_manager;
        physics_manager_count++;
    }

    void remove_physics_manager(Editor::PhysicsManager::physics_manager *py_manager) {
        bool found = false;
        for (int i = 0; i < physics_manager_count; i++) {
            if (&physics_manager_list[i] == py_manager) {
                found = true;
            }
            if (found && i < physics_manager_count - 1) {
                physics_manager_list[i] = physics_manager_list[i + 1];
            }
        }
        if (found) {
            physics_manager_count--;
        }
    }
}
