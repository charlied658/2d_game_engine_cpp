/**
 * @author Charlie Davidson 
 * Created on 7/11/22.
 */

#include "editor/holding_object.h"

#include "core/scene.h"
#include "core/mouse_listener.h"
#include "editor/selected_objects.h"
#include "editor/imgui/object_picker.h"
#include "render/sprite_renderer.h"
#include "util/collision_math.h"

namespace Holding {

    static GameObject::game_object holding_object;
    static bool generated_holding;
    static bool holding;

    static GameObject::game_object *game_object_list;
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
            holding_object.name = name;
            GameObject::set_visible(&holding_object, true);
            GameObject::set_sprite(&holding_object, spr);
            return;
        }
        GameObject::init(&holding_object, name, glm::vec2{}, glm::vec2{0.25f,0.25f}, 5, spr);
        GameObject::set_pickable(&holding_object, false);
        SpriteRenderer::add_game_object(&holding_object);
        generated_holding = true;
    }

    /**
     * Update the position of the holding object.
     */
    void update() {
        if (Holding::is_holding()) {
            GameObject::set_position(&holding_object, glm::vec2{
                Mouse::get_worldX() - holding_object.scale.x / 2,
                Mouse::get_worldY() - holding_object.scale.y / 2});
        }
    }

    /**
     * Place the holding object.
     * @param destroy Destroy holding object upon placing.
     */
    void place_holding_object(bool destroy) {
        GameObject::game_object generated = holding_object;
        generated.z_index = 0;
        GameObject::set_pickable(&generated, true);
        glm::vec2 centered_position = {holding_object.position.x + 0.25f / 2,
                                       holding_object.position.y + 0.25f / 2};
        glm::vec2 position = {centered_position.x - Math::f_mod(centered_position.x, 0.25f),
                              centered_position.y - Math::f_mod(centered_position.y, 0.25f)};
        GameObject::set_position(&generated, position);
        if (destroy) {
            GameObject::set_selected(&generated, true);
        }
        Scene::add_game_object(&generated);
        if (destroy) {
            Scene::get_game_objects_list(&game_object_list, &game_object_count);
            Selected::select_holding_object(&game_object_list[game_object_count - 1]);
            Holding::destroy_holding_object();
        }
    }

    /**
     * Destroy the holding object.
     */
    void destroy_holding_object() {
        GameObject::set_visible(&holding_object, false);
        Holding::set_holding(false);
        ObjectPicker::reset();
    }

    void get_holding_object(GameObject::game_object **obj) {
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
