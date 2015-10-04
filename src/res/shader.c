#include "shader.h"

//////////////
// Includes //
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../debug.h"
#include "file.h"

//////////
// Code //

// Attempting to load a shader from a given location on disk.
GLuint Lentil_Reso_loadShader(const char* path, GLenum shaderType, Lentil_Core_Error* pErr) {
    // Opening a file for the given path.
    FILE* shaderFile = fopen(path, "r");
    if (shaderFile == NULL) {
        pErr->code    = Lentil_Core_FILENOTFOUND;

        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Failed to open shader %s.\n", path);

        return 0;
    }

    // Loading the contents of the shaderFile.
    char* contents = Lentil_Reso_loadFileContents(shaderFile, false);
    fclose(shaderFile);

    // Attempting to create and compile the shader.
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (const GLchar* const*)&contents, NULL);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        if (Lentil_Core_debugLevel(-1) > 0) {
            GLint length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            char* compileLog = malloc(length * sizeof(char));
            glGetShaderInfoLog(shader, length, NULL, compileLog);

            printf("Failed to compile shader: %s\n", compileLog);

            free(compileLog);
        }

        pErr->code = Lentil_Core_SHADERCOMPILEFAILED;
        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

// Attempting to load a shader program from a given location on disk. It scans
// for shaders at the path + ".vert", ".geom", and ".frag" respective.
GLuint Lentil_Reso_loadShaderProgram(const char* path, Lentil_Core_Error* pErr) {
    const char** suffixes = malloc(3 * sizeof(const char*));
    GLenum* shaderTypes = malloc(3 * sizeof(GLenum));
    GLuint* shaders = malloc(3 * sizeof(GLuint));

    // Populating the array data.
    shaderTypes[0] = GL_VERTEX_SHADER;
    shaderTypes[1] = GL_GEOMETRY_SHADER;
    shaderTypes[2] = GL_FRAGMENT_SHADER;

    suffixes[0] = ".vert";
    suffixes[1] = ".geom";
    suffixes[2] = ".frag";

    // Loading all of the individual shaders.
    char* realPath = malloc((strlen(path) + 6) * sizeof(char));
    Lentil_Core_Error err;
    for (int i = 0; i < 3; i++) {
        sprintf(realPath, "%s%s", path, suffixes[i]);
        err = Lentil_Core_defaultError();

        shaders[i] = Lentil_Reso_loadShader(realPath, shaderTypes[i], &err);
        if (Lentil_Core_isError(err)) {
            if (err.code > 0 && err.code != Lentil_Core_FILENOTFOUND) {
                pErr->code = err.code;

                printf("Lentil_Core_loadShader error: %s\n", Lentil_Core_errorName(err));

                free(shaderTypes);
                free(suffixes);
                free(shaders);
                free(realPath);

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
        if (Lentil_Core_debugLevel(-1) > 0) {
            GLint length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

            char* linkLog = malloc(length * sizeof(char));
            glGetProgramInfoLog(program, length, NULL, linkLog);

            printf("Failed to link shader program: %s\n", linkLog);

            free(linkLog);
        }

        pErr->code    = Lentil_Core_PROGRAMLINKFAILED;
        glDeleteProgram(program);
        program = 0;
    }

    // Cleaning up data.
    for (int i = 0; i < 3; i++)
        if (shaders[i] > 0)
            glDeleteShader(shaders[i]);

    free(shaderTypes);
    free(suffixes);
    free(shaders);

    return program;
}
