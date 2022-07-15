/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#include "holding_object.h"

#include "editor/scene.h"
#include "core/mouse_listener.h"
#include "selected_objects.h"
#include "object_picker.h"
#include "editor/render/sprite_renderer.h"
#include "editor/collision/chunk_manager.h"
#include "editor/collision/collision_math.h"

#include "editor/sprite_system.h"

namespace Holding {

    static Editor::GameObject::game_object holding_object;
    static bool generated_holding;
    static bool holding;

    static Editor::GameObject::game_object *game_object_list;
    static int game_object_count;

    /**
     * Initialize holding object.
     */
    void init() {
        Holding::set_holding(false);
        generated_holding = false;
    }

    /**
     * Reload the holding object.
     */ 
    void reload() {
        generated_holding = false;
    }

    /**
     * Generate a holding object.
     * @param spr Sprite reference
     */
    void generateHoldingObject(Sprite::sprite *spr, const std::string& name) {
        Selected::reset_selected();
        Holding::set_holding(true);
        // Get the holding object
        if (generated_holding) {
            holding_object.spr_manager->name = name;
            Editor::SpriteManager::set_visible(holding_object.spr_manager, true);
            Editor::SpriteManager::set_sprite(holding_object.spr_manager, spr);
            return;
        }
        Editor::SpriteSystem::add_sprite_manager(&holding_object.spr_manager);
        Editor::SpriteManager::init(holding_object.spr_manager, name, glm::vec2{}, glm::vec2{0.25f, 0.25f}, 10, spr);
        holding_object.spr_manager->pickable = false;
        Editor::SpriteRenderer::add_sprite(holding_object.spr_manager);
        generated_holding = true;
    }

    /**
     * Update the position of the holding object.
     */
    void update() {
        //printf("Holding object update\n");
        if (Holding::is_holding()) {
            Editor::SpriteManager::set_position(holding_object.spr_manager, glm::vec2{
                Mouse::get_worldX() - holding_object.spr_manager->scale.x / 2,
                Mouse::get_worldY() - holding_object.spr_manager->scale.y / 2});
        }
    }

    /**
     * Place the holding object.
     * @param destroy Destroy holding object upon placing.
     */
    void place_holding_object(bool destroy) {

        // Check grid coordinates.
        glm::vec2 centered_position = {holding_object.spr_manager->position.x + 0.25f / 2,
                                       holding_object.spr_manager->position.y + 0.25f / 2};
        glm::vec2 position = {centered_position.x - Math::f_mod(centered_position.x, 0.25f),
                              centered_position.y - Math::f_mod(centered_position.y, 0.25f)};
        int grid_x = (int) (position.x / 0.25f);
        int grid_y = (int) (position.y / 0.25f);

        if (ChunkManager::is_solid_block(grid_x,grid_y)) {
            //printf("Cannot place block %d,%d\n", grid_x, grid_y);
            return;
        } else {
            //printf("Placed block %d,%d\n", grid_x, grid_y);
            ChunkManager::set_solid_block(grid_x, grid_y, true);
        }

        Editor::GameObject::game_object *generated;
        Editor::Scene::add_game_object(&generated);
        *generated = holding_object;
        Editor::SpriteSystem::add_sprite_manager(&generated->spr_manager);
        Editor::SpriteRenderer::add_sprite(generated->spr_manager);
        *generated->spr_manager = *holding_object.spr_manager;
        generated->spr_manager->game_object = generated;
        generated->spr_manager->z_index = 0;
        generated->spr_manager->pickable = true;
        generated->spr_manager->grid_x = grid_x;
        generated->spr_manager->grid_y = grid_y;
        generated->spr_manager->last_grid_x = grid_x;
        generated->spr_manager->last_grid_y = grid_y;
        Editor::SpriteManager::set_position(generated->spr_manager, position);
        generated->spr_manager->last_position = position;
        if (destroy) {
            Editor::SpriteManager::set_selected(generated->spr_manager, true);
            Editor::Scene::get_game_objects_list(&game_object_list, &game_object_count);
            Selected::select_holding_object(&game_object_list[game_object_count - 1]);
            Holding::destroy_holding_object();
        }
    }

    /**
     * Destroy the holding object.
     */
    void destroy_holding_object() {
        Editor::SpriteManager::set_visible(holding_object.spr_manager, false);
        Holding::set_holding(false);
        ObjectPicker::reset();
    }

    void get_holding_object(Editor::GameObject::game_object **obj) {
        *obj = &holding_object;
    }

    /**
     * Get if the mouse is holding an object.
     * @return True if holding
     */
    bool is_holding() {
        return holding;
    }

    /**
     * Set if the mouse is holding an object.
     * @param state Whether the mouse is holding
     */
    void set_holding(bool state) {
        holding = state;
    }
}
