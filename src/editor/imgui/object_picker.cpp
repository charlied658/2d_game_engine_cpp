/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/imgui/object_picker.h"

#include "imgui/imgui.h"
#include "core/spritesheet.h"
#include "editor/holding_object.h"

namespace ObjectPicker {

    // Blocks
    static Spritesheet::spritesheet spritesheet1;
    static Sprite::sprite *sprite_list1;
    static int selected_obj1;

    // Objects
    static Spritesheet::spritesheet spritesheet2;
    static Sprite::sprite *sprite_list2;
    static int selected_obj2;

    // Items
    static Spritesheet::spritesheet spritesheet3;
    static Sprite::sprite *sprite_list3;
    static int selected_obj3;


    /**
     * Initialize sprites to update as buttons.
     */
    void init() {
        // Blocks
        Spritesheet::init(&spritesheet1, 7,12,"assets/images/spritesheets/decorationsAndBlocks.png");
        sprite_list1 = new Sprite::sprite[81];
        for (int i = 0; i < 81; i++) {
            sprite_list1[i] = Spritesheet::get_sprite(&spritesheet1, i);
        }

        // Objects
        Spritesheet::init(&spritesheet2, 7,3,"assets/images/spritesheets/icons.png");
        sprite_list2 = new Sprite::sprite[16];
        for (int i = 0; i < 16; i++) {
            sprite_list2[i] = Spritesheet::get_sprite(&spritesheet2, i);
        }

        // Items
        Spritesheet::init(&spritesheet3, 7,5,"assets/images/items.png");
        sprite_list3 = new Sprite::sprite[34];
        for (int i = 0; i < 34; i++) {
            sprite_list3[i] = Spritesheet::get_sprite(&spritesheet3, i);
        }

        ObjectPicker::reset();
    }

    /**
     * Reset the selected block.
     */
    void reset() {
        selected_obj1 = -1;
        selected_obj2 = -1;
        selected_obj3 = -1;
    }

    /**
     * Render object picker window.
     */
    void imgui() {
        ImGui::Begin("Object Picker");
        if (ImGui::BeginTabBar("TabBar")) {
            if (ImGui::BeginTabItem("Blocks")) {
                dynamic_button_grid(&sprite_list1, 81, "block", &selected_obj1);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Objects")) {
                dynamic_button_grid(&sprite_list2, 16, "object", &selected_obj2);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Items")) {
                dynamic_button_grid(&sprite_list3, 34, "item", &selected_obj3);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    /**
     * Render a grid of image buttons that resizes with the window.
     * @param sprite_list List of sprites
     * @param sprite_count Number of sprites
     */
    void dynamic_button_grid(Sprite::sprite **sprite_list, int sprite_count, const std::string& list_name, int *selected_block) {
        Sprite::sprite *list = *sprite_list;
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec2 button_sz(40, 40);
        bool block_selected;
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        for (int n = 0; n < sprite_count; n++) {
            ImGui::PushID(n);
            block_selected = (*selected_block == n);
            if (block_selected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.6f, 0.9f, 1.0f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.6f, 1.0f));
            }
            Sprite::sprite spr = list[n];
            ImVec2 uv0 = ImVec2(spr.tex_coords.x, spr.tex_coords.y + spr.tex_scale.y);
            ImVec2 uv1 = ImVec2(spr.tex_coords.x + spr.tex_scale.x, spr.tex_coords.y);
            if (ImGui::ImageButton((void*)(intptr_t)(int)spr.texture_ID, button_sz, uv0, uv1)) {
                if (*selected_block == n) {
                    *selected_block = -1;
                    Holding::destroy_holding_object();
                } else {
                    ObjectPicker::reset();
                    *selected_block = n;
                    Holding::generateHoldingObject(&spr, list_name + std::to_string(n));
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
            if (n + 1 < sprite_count && next_button_x2 < window_visible_x2)
                ImGui::SameLine();
            ImGui::PopID();
        }
    }
}