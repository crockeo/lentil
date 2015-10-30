//////////////
// Includes //
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "core.h"
#include "rend.h"
#include "reso.h"

//////////
// Code //

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

// Running the game with the created GLFWwindow.
void run(GLFWwindow* window, const char* modelLoc) {
    // Choosing an appropriate debug level.
    Lentil_Core_debugLevel(3);

    // Testing a shader load.
    Lentil_Core_Error shaderErr = Lentil_Core_defaultError();
    GLuint program = Lentil_Reso_loadShaderProgram("res/shaders/test", &shaderErr);

    // Testing a texture load.
    Lentil_Core_Error textureErr = Lentil_Core_defaultError();
    GLuint texture = Lentil_Reso_loadTexture("res/imgs/salmon.png", &textureErr);

    // Testing a model load.
    Lentil_Core_Error modelErr = Lentil_Core_defaultError();
    Lentil_Reso_Model* model = Lentil_Reso_Model_new();
    Lentil_Reso_loadObjModelStr(modelLoc, model, &modelErr);

    // Testing a model render.
    Lentil_Core_Error mrErr = Lentil_Core_defaultError();
    Lentil_Rend_ModelRender* mr = Lentil_Rend_ModelRender_new(model, &mrErr);

    if (Lentil_Core_isError(shaderErr))
        printf("Shader: %s\n", Lentil_Core_errorName(shaderErr));

    if (Lentil_Core_isError(textureErr))
        printf("Texture: %s\n", Lentil_Core_errorName(textureErr));

    if (Lentil_Core_isError(modelErr))
        printf("Model: %s\n", Lentil_Core_errorName(modelErr));

    if (Lentil_Core_isError(mrErr))
        printf("Model Render: %s\n", Lentil_Core_errorName(mrErr));

    // Running a render loop.
    double lt = 0, ct = 0;
    GLenum openglError;
    glfwSetTime(0);

    while (!glfwWindowShouldClose(window)) {
        // Getting the delta time since the last time the loop ran.
        lt = ct;
        ct = glfwGetTime();

        // Preparing for an update / render.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rendering.
        Lentil_Rend_ModelRender_render(mr, texture, program);

        // Finishing up an update / render.
        if (Lentil_Core_debugLevel(-1) > 0) {
            openglError = glGetError();
            if (openglError != GL_NO_ERROR) {
                printf("OpenGL Error: %d\n", openglError);
                break;
            }
        }

        double dt = ct - lt;
        if (dt < 1 / 60.0)
            Lentil_Core_sleep(1 / 60.0 - dt);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleaning up the allocated resources.
    glDeleteProgram(program);
    glDeleteTextures(1, &texture);
    Lentil_Reso_Model_destroy(model);
    Lentil_Rend_ModelRender_destroy(mr);
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

    if (argc > 1)
        run(window, argv[1]);
    else
        run(window, "res/models/test.obj");
    destroy(window);

    return 0;
}
