//////////////
// Includes //
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "flags.h"
#include "core.h"
#include "game.h"
#include "rend.h"
#include "reso.h"

//////////
// Code //

// The speed of rotational acceleration.
const double MAX_SPEED    = 500.0;
const double ROT_ACCEL    = 250.0;
const double MIN_VELOCITY =   0.3;

// Checking if a value is within some margin of another value.
bool within(double value, double target, double margin) {
    return value > target - margin && value < target + margin;
}

// Initializing the game.
GLFWwindow* initialize() {
    // Initializing GLFW.
    if (!glfwInit()) {
        printf("Failed to initialize GLFW.\n");
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Testing", NULL, NULL);
    if (window == NULL) {
        printf("Failed to open GLFW window.");

        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    // Initializing OpenGL.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return NULL;
    }

    // Setting up OpenGL.
    glClearColor(0.3, 0.3, 0.3, 0.0);

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glAlphaFunc(GL_NEVER, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    glDepthFunc(GL_LEQUAL);

    glGetError();
    return window;
}

// Cleaning up the engine after it has finished running.
void destroy(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// The entry point to the application.
int main(int argc, const char** argv) {
    Lentil_ResConfig cfg = Lentil_parseFlags(argc, argv);

    GLFWwindow* window = initialize();
    if (window == NULL)
        return 1;

    Lentil_Core_Error err;
    Lentil_runGame(window, cfg, &err);
    if (Lentil_Core_isError(err)) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Failed to start the game: %s", Lentil_Core_errorName(err));
        return 1;
    }

    destroy(window);

    return 0;
}
