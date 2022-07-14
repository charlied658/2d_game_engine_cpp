/**
 * @author Charlie Davidson 
 * Created on 7/14/22.
 */

#pragma once

#include "physics_manager.h"

namespace PhysicsSystem {

    void init();

    void reload();

    void add_physics_manager(Editor::PhysicsManager::physics_manager *py_manager);

    void remove_physics_manager(Editor::PhysicsManager::physics_manager *py_manager);
}
