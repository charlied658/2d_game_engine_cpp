/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "game/interface/main_menu_bar.h"

#include "core/window.h"

#include "imgui/imgui.h"

namespace Game {
    namespace MainMenuBar {

        /**
         * Render the main menu bar along the top of the screen.
         */
        void imgui() {
            if (ImGui::BeginMainMenuBar()) {
                // Level loading / saving system
                if (ImGui::BeginMenu("Stop")) {
                    Window::change_scene(true);
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
        }
    }
}