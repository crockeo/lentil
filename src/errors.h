#ifndef _ERROR_H_
#define _ERROR_H_

//////////////
// Includes //
#include <stdbool.h>

//////////
// Code //

// A struct to contain advanced error handling for function calls.
typedef struct {
    int code;
    const char* message;
} Lentil_Core_Error;

// Creating a default non-error Lentil_Core_Error.
Lentil_Core_Error Lentil_Core_defaultError();

// Checking if a given error has an real error code.
bool Lentil_Core_isError(Lentil_Core_Error);

// Getting the name of a given error from its error code.
const char* Lentil_Core_errorName(Lentil_Core_Error);

#endif
