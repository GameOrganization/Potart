#include "WindowManager.h"

GLFWwindow* window = NULL;

static void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char *argv[])
{
    window = initWindow();
    if (!window)
    {
        terminateWindow(window);
        return 1;
    }
    glfwSetKeyCallback(window, keyHandler);

    //empty game loop for testing
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);    //show what we rendered
        glfwPollEvents();
    }

    terminateWindow(window);
    return 0;
}
