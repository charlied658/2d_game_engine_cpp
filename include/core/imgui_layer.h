/**
 * @author Charlie Davidson 
 * Created on 7/7/22.
 */

#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace ImGuiLayer {

    void init(GLFWwindow *window);

    void render();

}
