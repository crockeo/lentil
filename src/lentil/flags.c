#include "flags.h"

//////////////
// Includes //
#include <string.h>
#include <stdio.h>

//////////
// Code //

//
// What makes something a flag?
//
// --<flag>=<value>
//
// So pretty much a flag is just a thing where you can find a <flag> that is
// equal to the flag prescribed.

// Finding out if a given character string is the start of another flag.
bool Lentil_isFlag(const char* str, const char* flag) {
    if (str[0] != '-' || str[1] != '-')
        return false;

    int flen = strlen(flag);
    for (int i = 0; i < flen; i++)
        if (str[i + 2] != flag[i])
            return false;

    if (str[flen + 2] != '=')
        return false;

    return true;
}

// Parsing out the flags to the program to create the appropriate
// Lentil_ResConfig.
Lentil_ResConfig Lentil_parseFlags(int argc, const char** argv) {
    Lentil_ResConfig config;

    config.shaderPath  = "res/shaders/test";
    config.texturePath = "res/imgs/test.png";
    config.modelPath   = "res/models/test.obj";

    for (int i = 1; i < argc; i++) {
        if (Lentil_isFlag(argv[i], "shader"))
            config.shaderPath = argv[i] + 9;
        else if (Lentil_isFlag(argv[i], "texture"))
            config.texturePath = argv[i] + 10;
        else if (Lentil_isFlag(argv[i], "model"))
            config.modelPath = argv[i] + 8;
    }

    return config;
}
