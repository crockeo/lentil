#ifndef _LENTIL_GAME_H_
#define _LENTIL_GAME_H_

//////////////
// Includes //
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "core/errors.h"
#include "flags.h"

//////////
// Code //

// Running the main game loop.
void Lentil_runGame(GLFWwindow*, Lentil_ResConfig, Lentil_Core_Error*);

#endif
