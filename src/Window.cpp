#include "Window.h"
#include <iostream>

//Initialize to NULL to signify that there is no window
GLFWwindow* Window::ptr = NULL;

//Data structure to hold the screen resolution, not directly accessible through the api
struct res {
    int w;
    int h;
} resolution = {0, 0};

//Callback function to display error codes and descriptions from GLFW
static void error(int error, const char* description) {
    std::cerr << "Error " << error << ": " << description << std::endl;
}

int Window::create() {
    //Set the error callback for GLFW
    glfwSetErrorCallback(error);

    //If the window already exists, dont overwrite
    if (ptr)
        return 1;

    //If GLFW fails to initialize, return 1
    if (!glfwInit())
        return 1;

    //Attempt to create the fullscreen window with native resolution
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* display = glfwGetVideoMode(monitor);
    ptr = glfwCreateWindow(display->width, display->height, WINDOW_TITLE, monitor, NULL);

    //If window is null, return 1
    if (!ptr)
        return 1;

    //Update the resolution struct
    resolution.w = display->width;
    resolution.h = display->height;

    //Make the window the current OpenGL context
    glfwMakeContextCurrent(ptr);

    //Attempt to initialize GLEW, if an error occurred, return null
    if (glewInit() != GLEW_OK)
        return NULL;

    //Return 0 since no error occurred
    return 0;
}

void Window::destroy() {
    //Destroy window
    glfwDestroyWindow(ptr);
    //Reflect that the window has been destroyed
    ptr = NULL;
    resolution.w = 0;
    resolution.h = 0;
    //Terminate GLFW
    glfwTerminate();
}

int Window::width() {
    return resolution.w;
}

int Window::height() {
    return resolution.h;
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(ptr);
}

void Window::update() {
    glfwSwapBuffers(ptr);
    glfwPollEvents();
}
