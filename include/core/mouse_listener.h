/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#pragma once

namespace Mouse {

    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    bool get_mouse_pressed(int button);
}
