#ifndef _LENTIL_RESO_FILE_H_
#define _LENTIL_RESO_FILE_H_

//////////////
// Includes //
#include <stdbool.h>
#include <stdio.h>

#include "../core/errors.h"

//////////
// Code //

// Loading the entire contents of a file. It mallocs memory into the char*
// located in the char** provided into the function.
//
// If called with fast as true, it doesn't chop off any extra space at the end
// of the file read. It makes it less memory dense, but skips an expensive (and
// possibly unnecessary) step.
void Lentil_Reso_loadFileContents(FILE*, char**, bool);

// Consuming input from a FILE* until the EOF, '\n', or '\r' keys are reached.
void Lentil_Reso_consumeLine(FILE*);

// Loading a token into the provided char* from the FILE* with max length
// defined by the int.
//
// Returns true when an EOF has been encountered.
bool Lentil_Reso_loadToken(FILE*, char*, int);

#endif
