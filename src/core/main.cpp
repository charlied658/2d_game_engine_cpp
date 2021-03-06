/**
 * @author Charlie Davidson
 */

#include "window.h"

int main()
{
    // Initialize the window
    if (Window::init()) {
        return -1;
    }

    // Loop the window until it closes
    Window::loop();
    return 0;
}