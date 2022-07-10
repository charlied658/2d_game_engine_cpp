/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/info_window.h"

#include "imgui/imgui.h"

#include "core/window.h"
#include "editor/select_objects.h"

namespace InfoWindow {

    static bool show_demo = false;

    /**
     * Render the info window.
     */
    void imgui() {
        ImGui::Begin("Info");
        if (ImGui::BeginTabBar("TabBar"))
        {
            if (ImGui::BeginTabItem("Selected"))
            {
                SelectObjects::imgui();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Debug"))
            {
                ImGui::Text("FPS: %f", Window::get_fps());
                ImGui::Checkbox("Show ImGui Demo Window", &show_demo);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    /**
     * Get whether the ImGui Demo Window should show
     * @return ImGui Window showing or not
     */
    bool show_demo_window() {
        return show_demo;
    }
}
