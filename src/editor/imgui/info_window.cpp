/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/imgui/info_window.h"

#include "imgui/imgui.h"

#include "core/window.h"
#include "editor/object_manager.h"

namespace InfoWindow {

    /**
     * Render the info window.
     */
    void imgui() {
        ImGui::Begin("Info");
        if (ImGui::BeginTabBar("TabBar"))
        {
            if (ImGui::BeginTabItem("Selected"))
            {
                ObjectManager::imgui();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Debug"))
            {
                ImGui::Text("FPS: %f", Window::get_fps());
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}
