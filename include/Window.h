#ifndef WINDOW_H
#define WINDOW_H

//GLEW handles loading OpenGL functions
//Must be included before other OpenGL libraries
//GLEW_STATIC is defined for static linkage
#include "GL/glew.h"

//GLFW handles creating windows for OpenGL cross-platform
//Also can handle user input and timing
#include "GLFW/glfw3.h"

//This will be the title of the window
#define WINDOW_TITLE "Potart"

namespace Window {

//Pointer to the GLFWwindow object used
extern GLFWwindow* ptr;

//Creates a fullscreen window, initializes OpenGL libraries, returns zero if successful
int create();

//Safely closes the window and OpenGL libraries
void destroy();

//Functions to access the resolution with minimal overhead
int width();
int height();

//Utility functions for game loop
bool isOpen();
void update();

} //namespace Window

#endif // WINDOW_H
