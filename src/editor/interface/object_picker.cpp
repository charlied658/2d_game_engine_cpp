/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "object_picker.h"

#include <utility>

#include "imgui/imgui.h"
#include "core/spritesheet.h"
#include "holding_object.h"

namespace ObjectPicker {

    // Spritesheets
    static Spritesheet::spritesheet decorations_and_blocks;
    static Spritesheet::spritesheet icons;
    static Spritesheet::spritesheet items;

    static Spritesheet::spritesheet *current_spritesheet;
    static Sprite::sprite *current_list;
    static int *current_count;

    // Solid blocks
    static Sprite::sprite *solid_blocks;
    static int solid_block_count;
    static int selected_solid_block;

    // Objects
    static Sprite::sprite *objects;
    static int object_count;
    static int selected_object;

    // Decorations
    static Sprite::sprite *decorations;
    static int decoration_count;
    static int selected_decoration;

    // Misc
    static Sprite::sprite *misc_objects;
    static int misc_objects_count;
    static int selected_misc_object;


    /**
     * Initialize sprites to update as buttons.
     */
    void init() {
        // Initialize spritesheets
        Spritesheet::init(&decorations_and_blocks, 7, 12, "assets/images/spritesheets/decorationsAndBlocks.png");
        Spritesheet::init(&icons, 7, 3, "assets/images/spritesheets/icons.png");
        Spritesheet::init(&items, 7, 5, "assets/images/items.png");

        solid_block_count = 0;
        object_count = 0;
        decoration_count = 0;
        misc_objects_count = 0;

        // Solid Blocks
        solid_blocks = new Sprite::sprite[50];
        set_current_list(solid_blocks, &solid_block_count);
        set_current_spritesheet(&decorations_and_blocks);
        add_sprite(0);
        add_sprite(1);
        add_sprite(7);
        add_sprite(8);
        add_sprite(14);
        add_sprite(17);
        add_sprite(21);
        add_sprite(22);
        add_sprite(28);
        add_sprite(29);
        add_sprite(33);
        add_sprite(35);
        add_sprite(36);
        add_sprite(37);
        add_sprite(42);
        add_sprite(43);
        add_sprite(44);

        // Objects
        objects = new Sprite::sprite[20];
        set_current_list(objects, &object_count);
        set_current_spritesheet(&icons);
        add_sprite(0);
        add_sprite(1);
        add_sprite(15);
        for (int i = 5; i <=14; i++) {
            add_sprite(i);
        }
        set_current_spritesheet(&items);
        add_sprite(6);

        // Decorations
        decorations = new Sprite::sprite[50];
        set_current_list(decorations, &decoration_count);
        set_current_spritesheet(&decorations_and_blocks);
        add_sprite(34);
        add_sprite(38);
        add_sprite(39);
        add_sprite(40);
        add_sprite(41);
        add_sprite(45);
        add_sprite(46);
        add_sprite(47);
        add_sprite(48);
        for (int i = 49; i <= 60; i++) {
            add_sprite(i);
        }

        // Misc
        misc_objects = new Sprite::sprite[200];
        set_current_list(misc_objects, &misc_objects_count);
        set_current_spritesheet(&decorations_and_blocks);
        for (int i = 0; i < 81; i++) {
            add_sprite(i);
        }
        set_current_spritesheet(&icons);
        for (int i = 0; i < 16; i++) {
            add_sprite(i);
        }
        set_current_spritesheet(&items);
        for (int i = 0; i < 34; i++) {
            add_sprite(i);
        }

        ObjectPicker::reset();
    }

    /**
     * Reset the selected block.
     */
    void reset() {
        selected_solid_block = -1;
        selected_object = -1;
        selected_decoration = -1;
        selected_misc_object = -1;
    }

    /**
     * Render object picker window.
     */
    void imgui() {
        ImGui::Begin("Object Picker");
        if (ImGui::BeginTabBar("TabBar")) {
            if (ImGui::BeginTabItem("Solid Blocks")) {
                dynamic_button_grid(&solid_blocks, solid_block_count, "block", &selected_solid_block);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Objects")) {
                dynamic_button_grid(&objects, object_count, "object", &selected_object);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Decorations")) {
                dynamic_button_grid(&decorations, decoration_count, "item", &selected_decoration);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Misc")) {
                dynamic_button_grid(&misc_objects, misc_objects_count, "misc", &selected_misc_object);
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
                    Holding::generate_holding_object(&spr, list_name + std::to_string(n));
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

    void set_current_list(Sprite::sprite *list, int *count) {
        current_list = list;
        current_count = count;
    }

    void set_current_spritesheet(Spritesheet::spritesheet *spr_sheet) {
        current_spritesheet = spr_sheet;
    }

    void add_sprite(int spr_index) {
        Sprite::sprite spr = Spritesheet::get_sprite(current_spritesheet, spr_index);
        current_list[*current_count] = spr;
        *current_count = *current_count + 1;
    }
}