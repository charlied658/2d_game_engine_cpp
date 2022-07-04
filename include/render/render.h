/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#pragma once

namespace Render {

    void init();

    void draw();

    static void generate_vertex_data();

    static int get_texture_slot(unsigned int texture_ID);

}
