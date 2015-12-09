#include "lntsaver.h"

//////////////
// Includes //
#include "../core/debug.h"

//////////
// Code //

// Saving a Lentil_Reso_Model* to a given file on disk in the .lnt format, a
// file format made for lentil.
//
// For file spec information, check out the documentation in doc/lntmodel.txt
void Lentil_Reso_saveLntModel(FILE* file, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    if (file == NULL) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Invalid file pointer.\n");

        pErr->code = Lentil_Core_FILENOTFOUND;
        return;
    }

    // TODO: Save the model.
}

// Saving a Lentil_Reso_Model* to a given path on disk - opens a file and then
// calles Lentil_Reso_saveLntModel.
void Lentil_Reso_saveLntModelStr(const char* path, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    FILE* file = fopen(path, "w");
    Lentil_Reso_saveLntModel(file, model, pErr);
}
