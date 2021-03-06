/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#pragma once

namespace Window {

    int init();

    void loop();

    void close_window();

    int get_width();

    int get_height();

    double get_fps();

    float get_aspect_ratio();

    void change_scene(bool state);

    void imgui();

}
