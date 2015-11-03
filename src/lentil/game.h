#ifndef _LENTIL_GAME_H_
#define _LENTIL_GAME_H_

//////////////
// Includes //
#include <GL/glew.h>

#include "core/errors.h"
#include "reso/model.h"
#include "math/vectors.h"

//////////
// Code //

typedef struct {
    const char** texturePaths;
    int textureLen;

    const char** shaderPaths;
    int shaderLen;

    const char** modelPaths;
    int modelLen;

    Lentil_Math_V3* positions;
    int positionLen;
} Lentil_Game_SceneSpec;

// The datatype to represent a whole Lentil_Game.
typedef struct {
    // The rendering targets.
    Lentil_Reso_Model** models;
    int modelLen;

    GLuint* textures;
    int textureLen;
    
    GLuint* shaders;
    int shaderLen;

    Lentil_Math_V3* positions;
    int positionLen;

    // The perspective and position of the player.
    Lentil_Math_V3 rotation;
    Lentil_Math_V3 position;
} Lentil_Game;

// Constructing a new Lentil_Game from disk locations for textures, shaders, and
// models.
Lentil_Game* Lentil_Game_new(Lentil_Game_SceneSpec*, Lentil_Core_Error*);

// Freeing the memory associated with an instance of a Lentil_Game.
void Lentil_Game_destroy(Lentil_Game*);

// Updating a Lentil_Game* given a delta time (other inputs can be accessed
// directly through GLFW.)
void Lentil_Game_update(Lentil_Game*, float);

// Rendering a given state of a Lentil_Game*.
void Lentil_Game_render(Lentil_Game*);

// The loop surrounding the direct game logic to update and render. Keeps track
// of GLFW state and all of that stuff.
void Lentil_Game_run(Lentil_Game*);

#endif
