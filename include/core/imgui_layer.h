/**
 * @author Charlie Davidson 
 * Created on 7/7/22.
 */

#pragma once

#include "lib/imgui/imgui.h"
#include "lib/imgui/imgui_impl_glfw.h"
#include "lib/imgui/imgui_impl_opengl3.h"

namespace ImGuiLayer {

    void init(GLFWwindow *window);

    void render();

}
