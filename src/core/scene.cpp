/**
 * @author Charlie Davidson 
 * Created on 7/5/22.
 */

#include "core/scene.h"

#include "render/texture.h"
#include "core/game_object.h"
#include "render/render.h"

#include <cstdio>

namespace Scene {

    static unsigned int textureID ,textureID2;

    /**
     * Initialize the scene.
     */
    void init() {
        // Generate textures
        textureID = Texture::get_texture("../assets/images/testImage.png")->textureID;
        textureID2 = Texture::get_texture("../assets/images/testImage2.png")->textureID;

        // Generate game objects
        GameObject::game_object obj1 {"obj1", 2.0f, 1.0f, 1.0f, 1.0f, textureID};
        GameObject::game_object obj2 {"obj2", 3.0f, 1.0f, 1.0f, 1.0f, textureID2};

        // Add game objects to the scene
        Render::add_game_object(&obj1);
        Render::add_game_object(&obj2);
    }

    /**
     * Update the scene each frame.
     * @param dt Delta time
     */
    void update(double dt) {
        //printf("FPS: %f\n", 1/dt);
    }
}