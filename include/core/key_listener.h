/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#pragma once

namespace Key {

    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    bool get_key_pressed(int key);

    bool get_key_begin_press(int key);
}
