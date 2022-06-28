//
// Created by Charlie Davidson on 6/27/22.
//

#include <GLFW/glfw3.h>
#include "core/key_listener.h"

namespace Key {

    static bool key_pressed[GLFW_KEY_LAST];

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            key_pressed[key] = true;
        } else if (action == GLFW_RELEASE) {
            key_pressed[key] = false;
        }
    }

    bool get_key_pressed(int key) {
        return key_pressed[key];
    }
}
