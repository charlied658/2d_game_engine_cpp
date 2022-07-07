# 2D Game Engine (C++)
This is a 2D game engine prototype based off of my other project [2d_game_engine](https://github.com/charlied658/2d_game_engine), although written in C++ instead of Java.
- It also draws heavy inspiration from [Cocoa Engine](https://github.com/ambrosiogabe/Cocoa).
- C++ has better library support and memory management, which makes it better suited for a game engine.

Tools this project is built on:
- [X] [GLFW](https://www.glfw.org/) - Window handling
- [X] [OpenGL](https://www.opengl.org/) - Graphics rendering
- [X] [STB](https://github.com/nothings/stb) - Image loading
- [X] [Dear ImGui](https://github.com/ocornut/imgui) - User Interface
- [ ] [Box2D](https://box2d.org/) - Physics

## Running
After cloning this repository, navigate to `include/util/properties.h.example`.
Replace the `PROJECT_PATH` identifier with the absolute path of this project on your machine,
then remove the `.example` extension.
Example: `/Users/projects/.../gameEngine/`.
This allows the engine to access resources within the `assets` folder, like textures.
- You may also replace it with a relative path if you know where the executable
is generated within the project.
- Example: With CLion you can set it to `../`, with Visual Studio 
you can set it to `../../../`

I am running macOS and using the CLion IDE. 
You should be able to compile and run this project by 
running `cmake` using `CMakeLists.txt`

For information about navigating the game engine interface, see CONTROLS.md