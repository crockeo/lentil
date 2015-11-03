#include "game.h"

//////////////
// Includes //
#include <stdlib.h>
#include <stdio.h>

#include "core/debug.h"

//////////
// Code //

// Constructing a new Lentil_Game from disk locations for textures, shaders, and
// models.
Lentil_Game* Lentil_Game_new(Lentil_Game_SceneSpec* spec, Lentil_Core_Error* pErr) {
    // Allocating space for the game.
    Lentil_Game* game = malloc(sizeof(Lentil_Game));
    if (game == NULL) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Failed to malloc space for the game.\n");

        pErr->code = Lentil_Core_GAMECREATIONFAILED;
        return NULL;
    }

    // TODO: Fill the assets.


    // Filling the rest of the game data.
    game->positions = spec->positions;
    game->positionLen = spec->positionLen;

    game->rotation.x = 0;
    game->rotation.y = 0;

    game->position.x = 0;
    game->position.y = 0;
    game->position.z = 0;

    return game;
}

// Freeing the memory associated with an instance of a Lentil_Game.
void Lentil_Game_destroy(Lentil_Game* game) {
    for (int i = 0; i < game->modelLen; i++)
        Lentil_Reso_Model_destroy(game->models[i]);
    free(game->models);

    glDeleteTextures(game->textureLen, game->textures);
    free(game->textures);

    for (int i = 0; i < game->shaderLen; i++)
        glDeleteProgram(game->shaders[i]);
    free(game->shaders);

    free(game);
}

// Updating a Lentil_Game* given a delta time (other inputs can be accessed
// directly through GLFW.)
void Lentil_Game_update(Lentil_Game* game, float dt) {

}

// Rendering a given state of a Lentil_Game*.
void Lentil_Game_render(Lentil_Game* game) {

}

// The loop surrounding the direct game logic to update and render. Keeps track
// of GLFW state and all of that stuff.
void Lentil_Game_run(Lentil_Game* game) {

}
