#ifndef _LENTIL_RESO_FILE_H_
#define _LENTIL_RESO_FILE_H_

//////////////
// Includes //
#include <stdbool.h>
#include <stdio.h>

//////////
// Code //

// Loading the entire contents of a file.
//
// Generally considered bad practice, but this function returns a malloced
// char*, meaning that it has to be freed after the function is called.
//
// If called with fast as true, it doesn't chop off any extra space at the end
// of the file read. It makes it less memory dense, but skips an expensive (and
// possibly unnecessary) step.
char* Lentil_Reso_loadFileContents(FILE*, bool);

#endif
