//////////////
// Includes //
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "res/shader.h"
#include "debug.h"

//////////
// Code //

// The entry point to the application.
int main(int argc, char** argv) {
    // Initializing GLFW & OpenGL.
    if (!glfwInit()) {
        printf("Failed to initialize GLFW.\n");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Testing", NULL, NULL);
    if (window == NULL) {
        printf("Failed to open GLFW window.");

        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return 1;
    }

    // Testing some shader loads.
    Lentil_Core_debugLevel(3);

    Lentil_Core_Error err = Lentil_Core_defaultError();
    GLuint program = Lentil_Reso_loadShaderProgram("res/shaders/test", &err);

    // Cleaning up GLFW & OpenGL.
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
