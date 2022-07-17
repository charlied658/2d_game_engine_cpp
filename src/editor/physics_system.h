/**
 * @author Charlie Davidson 
 * Created on 7/14/22.
 */

#pragma once

#include "physics_manager.h"

namespace Editor {
    namespace PhysicsSystem {

        void init();

        void reload();

        void init_physics_manager(Editor::PhysicsManager::physics_manager **py_manager);

        void init_transient_physics_manager(Editor::PhysicsManager::physics_manager **py_manager);

        void remove_physics_manager(Editor::PhysicsManager::physics_manager *py_manager);

        void get_physics_manager_list(Editor::PhysicsManager::physics_manager **objects, int *object_count);

        void get_transient_physics_manager_list(Editor::PhysicsManager::physics_manager **objects, int *object_count);
    }
}
