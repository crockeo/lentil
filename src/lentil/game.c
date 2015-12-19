#include "game.h"

//////////////
// Includes //
#include <stdlib.h>
#include <stdio.h>

#include "rend/modelutils.h"
#include "reso/objloader.h"
#include "reso/texture.h"
#include "rend/camera.h"
#include "reso/shader.h"
#include "core/debug.h"
#include "core/sleep.h"
#include "reso/model.h"

//////////
// Code //

const static float ROTATION_SPEED     = 50.f;
const static float ACCELERATION_SPEED = 0.50f;
const static float MIN_SPEED          = 0.05f;
const static float MAX_SPEED          = 5.f;

// Checking that a number is within some range about a number.
float Lentil_within(float num, float target, float variance) {
    return num > target - variance && num > target + variance;
}

// The structure of the Lentil game.
typedef struct {
    Lentil_Rend_ModelRender* modelRender;
    Lentil_Reso_Model* model;
    GLuint texture, shader;

    float dx, dy, dz;
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

    game->dx = 0;
    game->dy = 0;
    game->dz = 0;

    return game;
}

// Destroying a game.
void Lentil_Game_destroy(Lentil_Game* game) {
    Lentil_Reso_Model_destroy(game->model);
    glDeleteTextures(1, &game->texture);
    glDeleteProgram(game->shader);
    free(game);
}

// Handling movement in a given direction.
void Lentil_Game_move(GLFWwindow* window, float dt, float* d, int pkey, int mkey) {
    if (glfwGetKey(window, pkey) == GLFW_PRESS)
        (*d) += ACCELERATION_SPEED * dt;
    else if (glfwGetKey(window, mkey) == GLFW_PRESS)
        (*d) -= ACCELERATION_SPEED * dt;
    else {
        if ((*d) > MIN_SPEED)
            (*d) -= ACCELERATION_SPEED * dt;
        else if ((*d) < -MIN_SPEED)
            (*d) += ACCELERATION_SPEED * dt;
        else
            (*d) = 0;

    }

    if ((*d) < -MAX_SPEED)
        (*d) = -MAX_SPEED;
    else if ((*d) > MAX_SPEED)
        (*d) = MAX_SPEED;
}

// Updating a game given the time since the last update (to normalize update
// speed).
void Lentil_Game_update(Lentil_Game* game, GLFWwindow* window, float dt) {
    // Rotating the camera.
    float dxrot = 0.f, dyrot = 0.f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        dxrot -= ROTATION_SPEED * dt;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        dxrot += ROTATION_SPEED * dt;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        dyrot -= ROTATION_SPEED * dt;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        dyrot += ROTATION_SPEED * dt;

    Lentil_Rend_Camera_rotate(game->modelRender->camera, dxrot, dyrot);

    // Translating the camera.
    Lentil_Game_move(window, dt, &game->dx, GLFW_KEY_D, GLFW_KEY_A);
    Lentil_Game_move(window, dt, &game->dy, GLFW_KEY_Q, GLFW_KEY_E);
    Lentil_Game_move(window, dt, &game->dz, GLFW_KEY_W, GLFW_KEY_S);

    Lentil_Rend_Camera_translate(game->modelRender->camera, game->dx, game->dy, game->dz);
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

        Lentil_Game_update(game, window, dt);
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
