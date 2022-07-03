/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include <GLFW/glfw3.h>
#include "core/mouse_listener.h"

namespace Mouse {
    static double xPos, yPos;
    static double scrollX, scrollY;
    static bool mouse_down[GLFW_MOUSE_BUTTON_LAST];

    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
        Mouse::xPos = xpos;
        Mouse::yPos = ypos;
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS) {
            mouse_down[button] = true;
        } else if (action == GLFW_RELEASE) {
            mouse_down[button] = false;
        }
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        Mouse::scrollX = xoffset;
        Mouse::scrollY = yoffset;
    }

    bool get_mouse_pressed(int button) {
        return Mouse::mouse_down[button];
    }
}
