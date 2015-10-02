#include "shader.h"

//////////
// Code //

// Attempting to load a shader from a given location on disk.
//
// Upon failure it will populate Lentil_Core_Error with the appropriate
// information.
GLuint Lentil_loadShader(const char* path, Lentil_Core_Error* pErr) {
    if (pErr == NULL)
        return 0;

    pErr->code    = 1;
    pErr->message = "not yet implemented.";

    return 0;
}
