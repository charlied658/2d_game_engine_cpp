/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/imgui/object_picker.h"

#include "imgui/imgui.h"

namespace ObjectPicker {

    static int selected_block = -1;

    /**
     * Render object picker window.
     */
    void imgui() {
        ImGui::Begin("Objects");
        ImVec2 button_sz(40, 40);
        if (ImGui::BeginTabBar("TabBar")) {
            if (ImGui::BeginTabItem("Blocks")) {
                ImGuiStyle &style = ImGui::GetStyle();
                int buttons_count = 20;
                bool block_selected;
                float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
                for (int n = 0; n < buttons_count; n++) {
                    ImGui::PushID(n);
                    block_selected = (selected_block == n);
                    if (block_selected) {
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.6f, 0.9f, 1.0f));
                    } else {
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.6f, 1.0f));
                    }
                    if (ImGui::Button("Box", button_sz)) {
                        if (selected_block == n) {
                            selected_block = -1;
                        } else {
                            selected_block = n;
                        }
                    }
                    if (block_selected) {
                        ImGui::PopStyleColor();
                        ImGui::PopStyleColor();
                    } else {
                        ImGui::PopStyleColor();
                    }
                    float last_button_x2 = ImGui::GetItemRectMax().x;
                    float next_button_x2 = last_button_x2 + style.ItemSpacing.x +
                                           button_sz.x; // Expected position if next button was on same line
                    if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
                        ImGui::SameLine();
                    ImGui::PopID();
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}