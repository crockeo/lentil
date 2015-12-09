#include "lntloader.h"

//////////////
// Includes //
#include "../core/debug.h"

//////////
// Code //

// Loading a Lentil_Reso_Model* from a given file on disk in the .lnt format, a
// file format made for lentil.
//
// For file spec information, check out the documentation in doc/lntmodel.txt
void Lentil_Reso_loadLntModel(FILE* file, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    if (file == NULL) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Invalid file pointer.\n");

        pErr->code = Lentil_Core_FILENOTFOUND;
        return;
    }

    // TODO
}

// Loading a Lentil_Reso_Model* from a given path on disk - opens a file and
// then calls Lentil_Reso_loadLntModel.
void Lentil_Reso_loadLntModelStr(const char* path, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    FILE* file = fopen(path, "r");
    Lentil_Reso_loadLntModel(file, model, pErr);
}
