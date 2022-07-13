/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#pragma once

#include "core/game_object.h"

namespace Highlight {

    void init();

    void reload();

    void update();

    static void reset();

    static void highlight_objects();

    void get_highlighted_object(GameObject::game_object **object);

    void get_highlighted_objects(GameObject::game_object ***objects, int *object_count);

    bool is_highlighted();

    bool is_selected();

}
