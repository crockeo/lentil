#include "game.h"

//////////////
// Includes //
#include <stdlib.h>
#include <stdio.h>

#include "rend/modelutils.h"
#include "reso/objloader.h"
#include "reso/texture.h"
#include "reso/shader.h"
#include "core/debug.h"
#include "core/sleep.h"
#include "reso/model.h"

//////////
// Code //

/*// Running the game with the created GLFWwindow.*/
/*void run(GLFWwindow* window, Lentil_ResConfig cfg) {*/
    /*// Choosing an appropriate debug level.*/
    /*Lentil_Core_debugLevel(3);*/

    /*// Testing a shader load.*/
    /*Lentil_Core_Error shaderErr = Lentil_Core_defaultError();*/
    /*GLuint program = Lentil_Reso_loadShaderProgram(cfg.shaderPath, &shaderErr);*/

    /*// Testing a texture load.*/
    /*Lentil_Core_Error textureErr = Lentil_Core_defaultError();*/
    /*GLuint texture = Lentil_Reso_loadTexture(cfg.texturePath, &textureErr);*/

    /*// Testing a model load.*/
    /*Lentil_Core_Error modelErr = Lentil_Core_defaultError();*/
    /*Lentil_Reso_Model* model = Lentil_Reso_Model_new();*/
    /*Lentil_Reso_loadObjModelStr(cfg.modelPath, model, &modelErr);*/

    /*// Testing a model render.*/
    /*Lentil_Core_Error mrErr = Lentil_Core_defaultError();*/
    /*Lentil_Rend_ModelRender* mr = Lentil_Rend_ModelRender_new(model, &mrErr);*/

    /*if (Lentil_Core_isError(shaderErr))*/
        /*printf("Shader: %s\n", Lentil_Core_errorName(shaderErr));*/

    /*if (Lentil_Core_isError(textureErr))*/
        /*printf("Texture: %s\n", Lentil_Core_errorName(textureErr));*/

    /*if (Lentil_Core_isError(modelErr))*/
        /*printf("Model: %s\n", Lentil_Core_errorName(modelErr));*/

    /*if (Lentil_Core_isError(mrErr))*/
        /*printf("Model Render: %s\n", Lentil_Core_errorName(mrErr));*/

    /*// Running a render loop.*/
    /*double lt = 0, ct = 0;*/
    /*GLenum openglError;*/
    /*glfwSetTime(0);*/

    /*double xrot = 0, yrot = 0;*/
    /*double xvel = 0, yvel = 0;*/

    /*while (!glfwWindowShouldClose(window)) {*/
        /*// Getting the delta time since the last time the loop ran.*/
        /*lt = ct;*/
        /*ct = glfwGetTime();*/

        /*// Preparing for an update / render.*/
        /*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

        /*// Rendering.*/
        /*Lentil_Rend_ModelRender_renderRot(mr, texture, program, xrot, yrot);*/

        /*// Updating the velocity & rotation.*/
        /*double dt = ct - lt;*/
        /*bool l, r, u, d;*/

        /*l = glfwGetKey(window, GLFW_KEY_A);*/
        /*r = glfwGetKey(window, GLFW_KEY_D);*/
        /*u = glfwGetKey(window, GLFW_KEY_W);*/
        /*d = glfwGetKey(window, GLFW_KEY_S);*/

        /*if (!l && !r) {*/
            /*if (within(xvel, 0, MIN_VELOCITY))*/
                /*xvel = 0;*/
            /*else if (xvel < 0)*/
                /*xvel += ROT_ACCEL * dt;*/
            /*else if (xvel > 0)*/
                /*xvel -= ROT_ACCEL * dt;*/
        /*} else {*/
            /*if (l)*/
                /*xvel -= ROT_ACCEL * dt;*/
            /*if (r)*/
                /*xvel += ROT_ACCEL * dt;*/
        /*}*/

        /*if (!u && !d) {*/
            /*if (within(yvel, 0, MIN_VELOCITY))*/
                /*yvel = 0;*/
            /*else if (yvel < 0)*/
                /*yvel += ROT_ACCEL * dt;*/
            /*else if (yvel > 0)*/
                /*yvel -= ROT_ACCEL * dt;*/
        /*} else {*/
            /*if (u)*/
                /*yvel -= ROT_ACCEL * dt;*/
            /*if (d)*/
                /*yvel += ROT_ACCEL * dt;*/
        /*}*/

        /*// Capping the velocity at a maximum speed.*/
        /*if (xvel < -MAX_SPEED)*/
            /*xvel = -MAX_SPEED;*/
        /*if (xvel >  MAX_SPEED)*/
            /*xvel =  MAX_SPEED;*/

        /*if (yvel < -MAX_SPEED)*/
            /*yvel = -MAX_SPEED;*/
        /*if (yvel >  MAX_SPEED)*/
            /*yvel =  MAX_SPEED;*/

        /*xrot += xvel * dt;*/
        /*while (xrot < 0)*/
            /*xrot += 360;*/
        /*while (xrot > 360)*/
            /*xrot -= 360;*/

        /*yrot += yvel * dt;*/
        /*while (yrot < 0)*/
            /*yrot += 360;*/
        /*while (yrot > 360)*/
            /*yrot -= 360;*/

        /*// Finishing up an update / render.*/
        /*if (Lentil_Core_debugLevel(-1) > 0) {*/
            /*openglError = glGetError();*/
            /*if (openglError != GL_NO_ERROR) {*/
                /*printf("OpenGL Error: %d\n", openglError);*/
                /*break;*/
            /*}*/
        /*}*/

        /*if (dt < 1 / 60.0)*/
            /*Lentil_Core_sleep(1 / 60.0 - dt);*/

        /*glfwSwapBuffers(window);*/
        /*glfwPollEvents();*/
    /*}*/

    /*// Cleaning up the allocated resources.*/
    /*glDeleteProgram(program);*/
    /*glDeleteTextures(1, &texture);*/
    /*Lentil_Reso_Model_destroy(model);*/
    /*Lentil_Rend_ModelRender_destroy(mr);*/
/*}*/

typedef struct {
    Lentil_Rend_ModelRender* modelRender;
    Lentil_Reso_Model* model;
    GLuint texture, shader;
} Lentil_Game;

// Creating a game.
Lentil_Game* Lentil_Game_new(Lentil_ResConfig cfg, Lentil_Core_Error* pErr) {
    Lentil_Game* game = malloc(sizeof(Lentil_Game));

    game->model = Lentil_Reso_Model_new();
    Lentil_Reso_loadObjModelStr(cfg.modelPath, game->model, pErr);
    if (Lentil_Core_isError(*pErr)) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Failed to load object model.\n");
        Lentil_Reso_Model_destroy(game->model);
        free(game);

        return NULL;
    }

    game->texture = Lentil_Reso_loadTexture(cfg.texturePath, pErr);
    if (Lentil_Core_isError(*pErr)) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Failed to load texture.\n");

        Lentil_Reso_Model_destroy(game->model);
        free(game);

        return NULL;
    }

    game->shader = Lentil_Reso_loadShaderProgram(cfg.shaderPath, pErr);
    if (Lentil_Core_isError(*pErr)) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Failed to load shader.\n");

        Lentil_Reso_Model_destroy(game->model);
        glDeleteTextures(1, &game->texture);
        free(game);

        return NULL;
    }

    game->modelRender = Lentil_Rend_ModelRender_new(game->model, pErr);
    if (Lentil_Core_isError(*pErr)) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Failed to consturct ModelRender.\n");

        Lentil_Reso_Model_destroy(game->model);
        glDeleteTextures(1, &game->texture);
        glDeleteProgram(game->shader);
        free(game);

        return NULL;
    }

    return game;
}

// Destroying a game.
void Lentil_Game_destroy(Lentil_Game* game) {
    Lentil_Reso_Model_destroy(game->model);
    glDeleteTextures(1, &game->texture);
    glDeleteProgram(game->shader);
    free(game);
}

// Updating a game given the time since the last update (to normalize update
// speed).
void Lentil_Game_update(Lentil_Game* game, float dt) {
    // TODO: Update
}

// Performing a render given the current state of the game.
void Lentil_Game_render(Lentil_Game* game, GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Lentil_Rend_ModelRender_render(game->modelRender, game->texture, game->shader);
}

// Running the main game loop.
void Lentil_runGame(GLFWwindow* window, Lentil_ResConfig cfg, Lentil_Core_Error* pErr) {
    Lentil_Game* game = Lentil_Game_new(cfg, pErr);
    if (Lentil_Core_isError(*pErr))
        return;

    double lt = 0.0, ct = 0.0, dt = 0.0;
    GLenum glErr;
    
    while (!glfwWindowShouldClose(window)) {
        lt = ct;
        ct = glfwGetTime();
        dt = ct - lt;

        Lentil_Game_update(game, dt);
        Lentil_Game_render(game, window);

        if (Lentil_Core_debugLevel(-1)) {
            glErr = glGetError();
            if (glErr != GL_NO_ERROR) {
                printf("OpenGL Error: %d\n", glErr);
                break;
            }
        }

        if (dt < 1 / 60.0)
            Lentil_Core_sleep(1 / 60.0 - dt);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Lentil_Game_destroy(game);
}
