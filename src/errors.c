#include "errors.h"

//////////
// Code //

// Creating a default non-error Lentil_Core_Error.
Lentil_Core_Error Lentil_Core_defaultError() {
    Lentil_Core_Error err;
    err.code = 0;
    return err;
}

// Checking if a given error has an real error code.
bool Lentil_Core_isError(Lentil_Core_Error err) {
    return err.code != 0;
}

// Getting the name of a given error from its error code.
const char* Lentil_Core_errorName(Lentil_Core_Error err) {
    switch (err.code) {
    case 0:
        return "No error";
    case Lentil_Core_FILENOTFOUND:
        return "File not found";
    case Lentil_Core_SHADERCOMPILEFAILED:
        return "Shader compile failed";
    case Lentil_Core_PROGRAMLINKFAILED:
        return "Program link failed";
    default:
        return "Unknown error";
    }
}
