//////////////
// Includes //
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "flags.h"
#include "core.h"
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

// Testing the lntsaver and lntloader to see if they're working.
void test_lntio(const char* path, Lentil_Reso_Model* original) {
    Lentil_Core_Error err = Lentil_Core_defaultError();
    Lentil_Reso_saveLntModelStr(path, original, &err);
    if (Lentil_Core_isError(err)) {
        printf("Failed to save lnt model: %s\n", Lentil_Core_errorName(err));
        return;
    }

    Lentil_Reso_Model* new = Lentil_Reso_Model_new();
    Lentil_Reso_loadLntModelStr(path, new, &err);
    if (Lentil_Core_isError(err)) {
        printf("Failed to load new lnt model: %s\n", Lentil_Core_errorName(err));
        return;
    }

    Lentil_Reso_Model_print(original);
    Lentil_Reso_Model_print(new);

    remove(path);
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

// Running the game with the created GLFWwindow.
void run(GLFWwindow* window, Lentil_ResConfig cfg) {
    // Choosing an appropriate debug level.
    Lentil_Core_debugLevel(3);

    // Testing a shader load.
    Lentil_Core_Error shaderErr = Lentil_Core_defaultError();
    GLuint program = Lentil_Reso_loadShaderProgram(cfg.shaderPath, &shaderErr);

    // Testing a texture load.
    Lentil_Core_Error textureErr = Lentil_Core_defaultError();
    GLuint texture = Lentil_Reso_loadTexture(cfg.texturePath, &textureErr);

    // Testing a model load.
    Lentil_Core_Error modelErr = Lentil_Core_defaultError();
    Lentil_Reso_Model* model = Lentil_Reso_Model_new();
    Lentil_Reso_loadObjModelStr(cfg.modelPath, model, &modelErr);

    test_lntio("model.lnt", model);

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

    double xrot = 0, yrot = 0;
    double xvel = 0, yvel = 0;

    while (!glfwWindowShouldClose(window)) {
        // Getting the delta time since the last time the loop ran.
        lt = ct;
        ct = glfwGetTime();

        // Preparing for an update / render.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rendering.
        Lentil_Rend_ModelRender_renderRot(mr, texture, program, xrot, yrot);

        // Updating the velocity & rotation.
        double dt = ct - lt;
        bool l, r, u, d;

        l = glfwGetKey(window, GLFW_KEY_A);
        r = glfwGetKey(window, GLFW_KEY_D);
        u = glfwGetKey(window, GLFW_KEY_W);
        d = glfwGetKey(window, GLFW_KEY_S);

        if (!l && !r) {
            if (within(xvel, 0, MIN_VELOCITY))
                xvel = 0;
            else if (xvel < 0)
                xvel += ROT_ACCEL * dt;
            else if (xvel > 0)
                xvel -= ROT_ACCEL * dt;
        } else {
            if (l)
                xvel -= ROT_ACCEL * dt;
            if (r)
                xvel += ROT_ACCEL * dt;
        }

        if (!u && !d) {
            if (within(yvel, 0, MIN_VELOCITY))
                yvel = 0;
            else if (yvel < 0)
                yvel += ROT_ACCEL * dt;
            else if (yvel > 0)
                yvel -= ROT_ACCEL * dt;
        } else {
            if (u)
                yvel -= ROT_ACCEL * dt;
            if (d)
                yvel += ROT_ACCEL * dt;
        }

        // Capping the velocity at a maximum speed.
        if (xvel < -MAX_SPEED)
            xvel = -MAX_SPEED;
        if (xvel >  MAX_SPEED)
            xvel =  MAX_SPEED;

        if (yvel < -MAX_SPEED)
            yvel = -MAX_SPEED;
        if (yvel >  MAX_SPEED)
            yvel =  MAX_SPEED;

        xrot += xvel * dt;
        while (xrot < 0)
            xrot += 360;
        while (xrot > 360)
            xrot -= 360;

        yrot += yvel * dt;
        while (yrot < 0)
            yrot += 360;
        while (yrot > 360)
            yrot -= 360;

        // Finishing up an update / render.
        if (Lentil_Core_debugLevel(-1) > 0) {
            openglError = glGetError();
            if (openglError != GL_NO_ERROR) {
                printf("OpenGL Error: %d\n", openglError);
                break;
            }
        }

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
int main(int argc, const char** argv) {
    Lentil_ResConfig cfg = Lentil_parseFlags(argc, argv);

    GLFWwindow* window = initialize();
    if (window == NULL)
        return 1;
    run(window, cfg);
    destroy(window);

    return 0;
}
