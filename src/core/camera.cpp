/**
 * @author Charlie Davidson
 * Created on 6/28/22.
 */

#include "core/camera.h"

#include <glm/gtc/matrix_transform.hpp> // glm::transform, glm::ortho

#include "core/window.h"

namespace Camera {
    static glm::vec2 position;
    static float zoom = 1.0f;
    static const float screen_width = 6.0f;
    static const float screen_height = 3.0f;
    static glm::mat4 inverseView, inverseProjection;

    void init() {
        position = {0.0f,0.0f};
        zoom = 1.0f;
    }

    /**
     * Controls camera movement.
     * @param dt Delta time
     */
    void update(double dt) {
        // Move camera
        if (Key::get_key_pressed(GLFW_KEY_LEFT) && !Key::get_key_pressed(GLFW_KEY_RIGHT)) {
            Camera::move_camera(glm::vec2(dt * 1.5, 0));
        }
        if (Key::get_key_pressed(GLFW_KEY_RIGHT) && !Key::get_key_pressed(GLFW_KEY_LEFT)) {
            Camera::move_camera(glm::vec2(-dt * 1.5, 0));
        }
        if (Key::get_key_pressed(GLFW_KEY_UP) && !Key::get_key_pressed(GLFW_KEY_DOWN)) {
            Camera::move_camera(glm::vec2(0, -dt * 1.5));
        }
        if (Key::get_key_pressed(GLFW_KEY_DOWN) && !Key::get_key_pressed(GLFW_KEY_UP)) {
            Camera::move_camera(glm::vec2(0, dt * 1.5));
        }

        // Zoom in/out
        if (!Key::get_key_pressed(GLFW_KEY_LEFT_CONTROL) && !Key::get_key_pressed(GLFW_KEY_LEFT_SUPER)) {
            if (Key::get_key_pressed(GLFW_KEY_W) && !Key::get_key_pressed(GLFW_KEY_S)) {
                Camera::scale_camera(1.01f);
            }
            if (Key::get_key_pressed(GLFW_KEY_S) && !Key::get_key_pressed(GLFW_KEY_W)) {
                Camera::scale_camera(1 / 1.01f);
            }
        }

        // Reset camera
        if (Key::get_key_begin_press(GLFW_KEY_SPACE)) {
            Camera::init();
        }
    }

    void move_camera(glm::vec2 dv) {
        position += dv / zoom;
    }

    void scale_camera(float scale) {
        zoom *= scale;
        if (zoom < 0.25f) {
            zoom = 0.25f;
        }
        if (zoom > 2.0f) {
            zoom = 2.0f;
        }
    }

    /**
     * Calculate the view matrix.
     * @return View matrix
     */
    glm::mat4 get_view() {
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(position.x, position.y, 0.0f));
        inverseView = glm::inverse(view);
        return view;
    }

    /**
     * Calculate the projection matrix.
     * @return Projection matrix
     */
    glm::mat4 get_projection() {
        glm::mat4 projection;

        float aspect_ratio = Window::get_aspect_ratio();
        if (aspect_ratio < 2.0f) {
            // Correct for aspect ratio
            float frameXOffset = (screen_width - (aspect_ratio * screen_height)) / 2;
            float frameYOffset = 0;
            float frame_width = (aspect_ratio * screen_height);
            float frame_height = screen_height;

            // Correct for zoom
            float zoomXOffset = frameXOffset + frame_width * (1.0f - 1.0f / zoom) / 2;
            float zoomYOffset = frameYOffset + frame_height * (1.0f - 1.0f / zoom) / 2;
            float zoom_width = frame_width / zoom;
            float zoom_height = frame_height / zoom;

            projection = glm::ortho(zoomXOffset, zoom_width + zoomXOffset, zoomYOffset,
                                    zoom_height + zoomYOffset);
            inverseProjection = glm::inverse(projection);
            return projection;
        } else {
            // Correct for aspect ratio
            float frameXOffset = 0;
            float frameYOffset = (screen_height - (screen_width / aspect_ratio)) / 2;
            float frame_width = screen_width;
            float frame_height = (screen_width / aspect_ratio);

            // Correct for zoom
            float zoomXOffset = frameXOffset + frame_width * (1.0f - 1.0f / zoom) / 2;
            float zoomYOffset = frameYOffset + frame_height * (1.0f - 1.0f / zoom) / 2;
            float zoom_width = frame_width / zoom;
            float zoom_height = frame_height / zoom;

            projection = glm::ortho(zoomXOffset, zoom_width + zoomXOffset, zoomYOffset,
                                    zoom_height + zoomYOffset);
            inverseProjection = glm::inverse(projection);
            return projection;
        }
    }

    /**
     * Get the inverse view matrix.
     * @return Inverse view matrix
     */
    glm::mat4 get_inverseView() {
        return inverseView;
    }

    /**
     * Get the inverse projection matrix.
     * @return Inverse projection matrix
     */
    glm::mat4 get_inverseProjection() {
        return inverseProjection;
    }

    /**
     * Get the zoom level of the camera.
     * @return Zoom level
     */
    float getZoom() {
        return zoom;
    }

    /**
     * Get the position of the camera.
     * @return Camera position
     */
    glm::vec2 get_position() {
        return position;
    }
}
