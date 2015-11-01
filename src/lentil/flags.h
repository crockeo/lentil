#ifndef _LENTIL_FLAGS_H_
#define _LENTIL_FLAGS_H_

//////////////
// Includes //
#include <stdbool.h>

//////////
// Code //

// A configuration for which resources should be loaded.
typedef struct {
    const char* shaderPath;
    const char* texturePath;
    const char* modelPath;
} Lentil_ResConfig;

// Finding out if a given character string is the start of another flag.
bool Lentil_isFlag(const char*, const char*);

// Parsing out the flags to the program to create the appropriate
// Lentil_ResConfig.
Lentil_ResConfig Lentil_parseFlags(int, const char**);

#endif
