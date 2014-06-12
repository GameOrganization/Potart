#include "WindowManager.h"
#include <iostream>

/*
 * Error callback for GLFW
 * Prints the error code and description to cerr
 */
static void error(int error, const char* description)
{
    std::cerr << "Error " << error << ": " << description << std::endl;
}

GLFWwindow* initWindow()
{
    //Set the error callback for GLFW
    glfwSetErrorCallback(error);

    //If GLFW fails to initialize, return false
    if (!glfwInit())
        return NULL;


    //Attempt to create the window
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* display = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(display->width, display->height, WINDOW_TITLE, monitor, NULL);

    //If window is null, return false
    if (!window)
        return NULL;

    //Make the window the current OpenGL context
    glfwMakeContextCurrent(window);

    //Attempt to initialize GLEW, if an error occurred, return false
    if (glewInit() != GLEW_OK)
        return NULL;

    //Return true since no error occurred
    return window;
}

/*
 * Closes the window safely
 * Terminates GLFW
 */
void terminateWindow(GLFWwindow* window)
{
    //Destroy the window object
    glfwDestroyWindow(window);

    //Terminate GLFW
    glfwTerminate();
}
