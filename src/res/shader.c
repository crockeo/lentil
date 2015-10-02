#include "shader.h"

//////////////
// Includes //
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../debug.h"

//////////
// Code //

// Attempting to load a shader from a given location on disk.
GLuint Lentil_Reso_loadShader(const char* path, Lentil_Core_Error* pErr) {
    if (pErr == NULL)
        return 0;

    pErr->code    = 1;
    pErr->message = "not yet implemented.";

    return 0;
}

// Attempting to load a shader program from a given location on disk. It scans
// for shaders at the path + ".vert", ".geom", and ".frag" respective.
GLuint Lentil_Reso_loadShaderProgram(const char* path, Lentil_Core_Error* pErr) {
    const char** suffixes = (const char**)malloc(3 * sizeof(const char*));
    GLuint* shaders = (GLuint*)malloc(3 * sizeof(GLuint));

    suffixes[0] = ".vert";
    suffixes[1] = ".geom";
    suffixes[2] = ".frag";

    // Loading all of the individual shaders.
    char* realPath = (char*)malloc((strlen(path) + 6) * sizeof(char));
    Lentil_Core_Error err;
    for (int i = 0; i < 3; i++) {
        sprintf("%s%s", realPath, path, suffixes[i]);
        err = Lentil_Core_defaultError();

        shaders[i] = Lentil_Reso_loadShader(realPath, &err);
        if (Lentil_Core_isError(err)) {
            switch (err.code) {
                case Lentil_Core_FILENOTFOUND:
                    if (Lentil_Core_debugLevel(-1) > 0)
                        printf("%s: %s", Lentil_Core_errorName(err), err.message);

                    break;
                default:
                    *pErr = err;

                    free(realPath);
                    free(suffixes);

                    for (int j = 0; j < i; i++)
                        if (shaders[j] > 0)
                            glDeleteShader(shaders[j]);
                    free(shaders);

                    return 0;
            }
        }
    }
    free(realPath);

    // Constructing the shader program.
    GLuint program = glCreateProgram();

    for (int i = 0; i < 3; i++)
        if (shaders[i] > 0)
            glAttachShader(program, shaders[i]);
    glLinkProgram(program);

    // Checking for link errors.
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        pErr->code    = Lentil_Core_PROGRAMLINKFAILED;
        pErr->message = "Failed to link shader.";

        glDeleteProgram(program);
        program = 0;
    }

    // Cleaning up data.
    for (int i = 0; i < 3; i++)
        if (shaders[i] > 0)
            glDeleteShader(shaders[i]);

    free(suffixes);
    free(shaders);

    return program;
}
