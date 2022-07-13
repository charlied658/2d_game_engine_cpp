/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "core/mouse_listener.h"
#include "core/key_listener.h"

#include "glm/vec2.hpp" // glm::vec2

namespace Mouse {

    void update();

    static void mouse_click();

    static void shift_click();

    static void mouse_release();

    void update_drag_objects();

    void update_multiselect();

    bool is_mouse_down();

}
