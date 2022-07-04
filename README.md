# 2D Game Engine (C++)
This is a 2D game engine prototype based off of my other project [2d_game_engine](https://github.com/charlied658/2d_game_engine), although written in C++ instead of Java.
- C++ has better library support and memory management, which makes it better suited for a game engine.

Tools this project is built on:
- [X] [GLFW](https://www.glfw.org/) - Window handling
- [X] [OpenGL](https://www.opengl.org/) - Graphics rendering
- [ ] [Dear ImGui](https://github.com/ocornut/imgui) - User Interface
- [ ] [Box2D](https://box2d.org/) - Physics

## Running
The file "stb_image.h" is not included in this project because it is too large (~7,000 lines) and I did not write it.
To properly compile this project, download the file [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
and place it into the path: `include/lib/stb_image.h` within this project.

For information about navigating the game engine interface, see CONTROLS.md