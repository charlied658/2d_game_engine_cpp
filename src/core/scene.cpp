/**
 * @author Charlie Davidson 
 * Created on 7/5/22.
 */

#include "core/scene.h"

#include "render/texture.h"
#include "render/render.h"
#include "core/game_object.h"
#include "core/sprite.h"
#include "core/spritesheet.h"

#include <cstdio>

namespace Scene {

    static GameObject::game_object game_objects[1000];
    static int game_object_count = 0;

    static unsigned int textureID1, textureID2;
    static Spritesheet::spritesheet spritesheet1, spritesheet2;
    static Sprite::sprite sprite1, sprite2, sprite3, sprite4;
    static GameObject::game_object obj1, obj2;

    static double frameTime = 0.2f;
    static double frameTimeLeft = 0.0f;
    static int frame = 0;

    /**
     * Initialize the scene.
     */
    void init() {
        // Generate textures
        textureID1 = Texture::get_texture("../assets/images/spritesheet.png")->textureID;

        // Create spritesheet
        spritesheet1 = Spritesheet::spritesheet {14,2,textureID1};

        // Create sprites
        sprite1 = Spritesheet::get_sprite(&spritesheet1, 0);
        sprite2 = Spritesheet::get_sprite(&spritesheet1, 14);

        // Generate game objects
        GameObject::init(&obj1, "obj1", 2.0f, 1.0f, 1.0f, 1.0f, 1, &sprite1);
        GameObject::init (&obj2, "obj2", 3.0f, 1.0f, 1.0f, 1.0f, 0, &sprite2);

        // Add game objects to the scene
        Scene::add_game_object(&obj1);
        Scene::add_game_object(&obj2);

    }

    /**
     * Update the scene each frame.
     * @param dt Delta time
     */
    void update(double dt) {
        //printf("FPS: %f\n", 1/dt);
    }

    /**
     * Add a game object to the scene.
     * @param obj Game object reference
     */
    void add_game_object(GameObject::game_object *obj) {
        game_objects[game_object_count] = *obj;
        game_object_count++;
        Render::add_game_object(obj);
    }
}