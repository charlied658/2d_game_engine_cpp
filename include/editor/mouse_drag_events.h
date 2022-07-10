/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "glm/vec2.hpp" // glm::vec2

namespace Mouse {

    void init();

    void start_frame();

    void move_objects();

    void start_multiselect();

    bool is_multiselect();

    bool is_dragging_objects();

    bool is_shift_click_down();

    void set_multiselect(bool state);

    void set_dragging_objects(bool state);

    void set_shift_click_down(bool state);

}
