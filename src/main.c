//////////////
// Includes //
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "core/debug.h"

#include "rend/modelutils.h"

#include "reso/objloader.h"
#include "reso/texture.h"
#include "reso/shader.h"
#include "reso/model.h"

//////////
// Code //

// Initializing the game.
GLFWwindow* initialize() {
    // Initializing GLFW & OpenGL.
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

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return NULL;
    }

    return window;
}

// Running the game with the created GLFWwindow.
void run(GLFWwindow* window) {
    // Choosing an appropriate debug level.
    Lentil_Core_debugLevel(3);

    // Testing a shader load.
    Lentil_Core_Error shaderErr = Lentil_Core_defaultError();
    GLuint program = Lentil_Reso_loadShaderProgram("res/shaders/test", &shaderErr);

    // Testing a texture load.
    Lentil_Core_Error textureErr = Lentil_Core_defaultError();
    GLuint texture = Lentil_Reso_loadTexture("res/imgs/test.png", &textureErr);

    // Testing a model load.
    Lentil_Core_Error modelErr = Lentil_Core_defaultError();
    Lentil_Reso_Model* model = Lentil_Reso_Model_new();
    Lentil_Reso_loadObjModelStr("res/models/test.obj", model, &modelErr);

    printf("Shader: %s\n", Lentil_Core_errorName(shaderErr));
    printf("Texture: %s\n", Lentil_Core_errorName(textureErr));
    printf("Model: %s\n", Lentil_Core_errorName(modelErr));
    Lentil_Reso_Model_print(model);

    // Running a render loop.
    Lentil_Core_Error renderErr;
    double lt = 0, ct = 0;
    glfwSetTime(0);
    while (!glfwWindowShouldClose(window)) {
        // Getting the delta time since the last time the loop ran.
        lt = ct;
        ct = glfwGetTime();

        // Preparing for an update / render.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rendering.
        Lentil_Rend_renderModel(model, texture, program, &renderErr);

        // Finishing up an update / render.
        // TODO: Portable thread sleep across operating system.
        if (ct - lt < 1 / 60.0) { }

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    // Cleaning up the allocated resources.
    glDeleteProgram(program);
    glDeleteTextures(1, &texture);
    Lentil_Reso_Model_destroy(model);
}

// Cleaning up the engine after it has finished running.
void destroy(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// The entry point to the application.
int main(int argc, char** argv) {
    GLFWwindow* window = initialize();
    if (window == NULL)
        return 1;
    run(window);
    destroy(window);

    return 0;
}
