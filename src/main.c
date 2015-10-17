//////////////
// Includes //
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "core/debug.h"

#include "reso/objloader.h"
#include "reso/texture.h"
#include "reso/shader.h"
#include "reso/model.h"

//////////
// Code //

// The entry point to the application.
int main(int argc, char** argv) {
    // Initializing GLFW & OpenGL.
    if (!glfwInit()) {
        printf("Failed to initialize GLFW.\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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
    Lentil_Reso_Model_print(model);
    Lentil_Reso_Model_destroy(model);

    printf("Shader: %s\n", Lentil_Core_errorName(shaderErr));
    printf("Texture: %s\n", Lentil_Core_errorName(textureErr));
    printf("Model: %s\n", Lentil_Core_errorName(modelErr));

    glDeleteProgram(program);
    glDeleteTextures(1, &texture);

    // Cleaning up GLFW & OpenGL.
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
