/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include <GLFW/glfw3.h>
#include "core/mouse_listener.h"

#include "core/window.h"
#include "core/camera.h"

namespace Mouse {
    static double xPos, yPos;
    static double worldX, worldY;
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

    double get_xPos() {
        return xPos;
    }

    double get_yPos() {
        return yPos;
    }

    double get_screenX() {
        return (xPos / Window::get_width()) * 2.0f - 1.0f;
    }

    double get_screenY() {
        return -(yPos / Window::get_height()) * 2.0f + 1.0f;
    }

    void calculate_world_coords() {
        double screenX = get_screenX();
        double screenY = get_screenY();
        glm::mat4 inverseProjection = Camera::get_inverseProjection();
        glm::mat4 inverseView = Camera::get_inverseView();

        glm::vec4 screen_coords {screenX, screenY, 0.0f, 1.0f};
        glm::vec4 world_coords = inverseView * inverseProjection * screen_coords;

        worldX = world_coords.x;
        worldY = world_coords.y;
    }

    double get_worldX() {
        return worldX;
    }

    double get_worldY() {
        return worldY;
    }
}
