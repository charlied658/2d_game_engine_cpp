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

    static unsigned int textureID1 ,textureID2;
    Spritesheet::spritesheet spritesheet1, spritesheet2;
    Sprite::sprite sprite1, sprite2, sprite3, sprite4;

    /**
     * Initialize the scene.
     */
    void init() {
        // Generate textures
        textureID1 = Texture::get_texture("../assets/images/spritesheet.png")->textureID;
        textureID2 = Texture::get_texture("../assets/images/turtle.png")->textureID;

        // Create spritesheet
        spritesheet1 = Spritesheet::spritesheet {14,2,textureID1};
        spritesheet2 = Spritesheet::spritesheet {4,1,textureID2};

        // Create sprites
        sprite1 = Spritesheet::get_sprite(&spritesheet1, 0);
        sprite2 = Spritesheet::get_sprite(&spritesheet1, 14);
        sprite3 = Spritesheet::get_sprite(&spritesheet1, 5);
        sprite4 = Spritesheet::get_sprite(&spritesheet2, 0);

        // Generate game objects
        GameObject::game_object obj1 {"obj1", 2.0f, 1.0f, 1.0f, 1.0f, sprite1};
        GameObject::game_object obj2 {"obj2", 3.0f, 1.0f, 1.0f, 1.0f, sprite2};
        GameObject::game_object obj3 {"obj3", 4.0f, 1.0f, 1.0f, 1.0f, sprite3};
        GameObject::game_object obj4 {"obj4", 1.0f, 1.0f, 1.0f, 1.5f, sprite4};

        // Add game objects to the scene
        Render::add_game_object(&obj1);
        Render::add_game_object(&obj2);
        Render::add_game_object(&obj3);
        Render::add_game_object(&obj4);
    }

    /**
     * Update the scene each frame.
     * @param dt Delta time
     */
    void update(double dt) {
        //printf("FPS: %f\n", 1/dt);
    }
}