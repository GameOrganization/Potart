#include "Window.h"

//Callback function for handling key events
static void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char *argv[]) {
    //Attempt to create window
    if (Window::create()) {
        Window::destroy();
        return 1;
    }

    //Set the key handler callback
    glfwSetKeyCallback(Window::ptr, keyHandler);

    //Enable vsync so mike's computer doesn't shit itself
    glfwSwapInterval(1);

    //Empty game loop for testing
    while (Window::isOpen()) {
        Window::update();
    }

    //Close the window
    Window::destroy();
    return 0;
}
