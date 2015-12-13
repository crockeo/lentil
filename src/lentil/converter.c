//////////////
// Includes //
#include <string.h>
#include <stdio.h>

#include "core.h"
#include "reso.h"

//////////
// Code //

// Printing the correct usage string.
void printCorrect() {
    printf("Proper usage: lentil-converter <source .obj> <destination .lnt> [v|verbose]\n");
}

// Entry point to the program.
int main(int argc, char** argv) {
    if (argc < 3 || argc > 4) {
        printCorrect();
        return 1;
    }

    // Ensuring we have verbose error messages if the user chooses.
    if (argc == 4) {
        if (strcmp(argv[3], "v") == 0 || strcmp(argv[3], "verbose") == 0)
            Lentil_Core_debugLevel(3);
        else {
            printCorrect();
            return 1;
        }
    }

    // Creating an error to use in model loading & saving.
    Lentil_Core_Error err = Lentil_Core_defaultError();

    // Loading the original model.
    Lentil_Reso_Model* model = Lentil_Reso_Model_new();
    Lentil_Reso_loadObjModelStr(argv[1], model, &err);
    if (Lentil_Core_isError(err)) {
        printf("Failed to load model: %s", Lentil_Core_errorName(err));
        return 1;
    }

    // Saving the new model.
    Lentil_Reso_saveLntModelStr(argv[2], model, &err);
    if (Lentil_Core_isError(err)) {
        printf("Failed to save .lnt model: %s", Lentil_Core_errorName(err));
        return 1;
    }
}
