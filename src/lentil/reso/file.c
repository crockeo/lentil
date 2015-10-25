#include "file.h"

//////////////
// Includes //
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//////////
// Code //

// Loading the entire contents of a file. It mallocs memory into the char*
// located in the char** provided into the function.
//
// If called with fast as true, it doesn't chop off any extra space at the end
// of the file read. It makes it less memory dense, but skips an expensive (and
// possibly unnecessary) step.
void Lentil_Reso_loadFileContents(FILE* file, char** contents, bool fast) {
    int size = 16;

    (*contents) = malloc(16 * sizeof(char));
    int pos = fread(*contents, 1, size, file);

    while (!feof(file)) {
        char* nContents = malloc(size * 2 * sizeof(char));
        for (int i = 0; i < size; i++)
            nContents[i] = (*contents)[i];
        free(*contents);
        (*contents) = nContents;
        size *= 2;

        pos += fread((*contents) + (size / 2), 1, size / 2, file);
    }
    (*contents)[pos] = '\0';

    if (!fast) {
        int len = strlen(*contents) + 1;

        char* nContents = malloc(len * sizeof(char));
        for (int i = 0; i < len; i++)
            nContents[i] = (*contents)[i];
        free(*contents);
        (*contents) = nContents;
    }
}

// Consuming input from a FILE* until the EOF, '\n', or '\r' keys are reached.
void Lentil_Reso_consumeLine(FILE* file) {
    if (file == NULL)
        return;

    int c;
    do {
        c = fgetc(file);
    } while (c != EOF && c != '\n' && c != '\r');
    if (c != EOF)
        ungetc(c, file);
}

// Loading a token into the provided char* from the FILE* with max length
// defined by the int.
bool Lentil_Reso_loadToken(FILE* file, char* str, int len) {
    bool consuming = true;

    int n = 0, c;
    while (n < len - 1 && (c = fgetc(file)) != EOF) {
        if (consuming) {
            if (isspace(c))
                continue;
            consuming = false;
        }

        if (isspace(c)) {
            ungetc(c, file);
            break;
        }

        str[n++] = c;
    }
    str[n] = '\0';

    return c == EOF;
}
