#include "file.h"

//////////////
// Includes //
#include <stdlib.h>
#include <string.h>

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
char* Lentil_Reso_loadFileContents(FILE* file, bool fast) {
    int size = 16;

    char* contents = malloc(16 * sizeof(char));
    int pos = fread(contents, 1, size, file);

    while (!feof(file)) {
        char* nContents = malloc(size * 2 * sizeof(char));
        for (int i = 0; i < size; i++)
            nContents[i] = contents[i];
        free(contents);
        contents = nContents;
        size *= 2;

        pos += fread(contents + (size / 2), 1, size / 2, file);
    }
    contents[pos] = '\0';

    if (!fast) {
        int len = strlen(contents) + 1;

        char* nContents = malloc(len * sizeof(char));
        for (int i = 0; i < len; i++)
            nContents[i] = contents[i];
        free(contents);
        contents = nContents;
    }

    return contents;
}
