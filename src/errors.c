#include "errors.h"

//////////
// Code //

// Creating a default non-error Lentil_Core_Error.
Lentil_Core_Error Lentil_Core_defaultError() {
    Lentil_Core_Error err;

    err.code    = 0;
    err.message = "";

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
    default:
        return "Unknown error";
    }
}
