#ifndef OPENGL_H_INCLUDED
#define OPENGL_H_INCLUDED

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#define WINDOW_TITLE "Potart"

/*
 * Initializes GLFW
 * Creates a full-screen OpenGL window
 * Initializes GLEW
 * Returns a pointer to the GLFWwindow object, or null if an error occurred
 */
GLFWwindow* initWindow();

/*
 * Closes the window safely
 * Terminates GLFW
 */
void terminateWindow(GLFWwindow* window);

//TODO more window-related functions

#endif // OPENGL_H_INCLUDED
