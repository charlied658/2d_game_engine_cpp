/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "core/mouse_listener.h"
#include "core/key_listener.h"

#include "glm/vec2.hpp" // glm::vec2

namespace Mouse {

    void init();

    void start_frame();

    void update();

    static void mouse_click();

    static void shift_click();

    static void mouse_release();

    void move_objects();

    void start_multiselect();

    bool is_multiselect();

    bool is_dragging_objects();

    bool is_shift_click_down();

    bool is_invalid_placement();

    void set_multiselect(bool state);

    void set_dragging_objects(bool state);

    void set_shift_click_down(bool state);

    void set_invalid_placement(bool state);

}
