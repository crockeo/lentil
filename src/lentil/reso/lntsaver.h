#ifndef _LENTIL_RESO_LNTSAVER_H_
#define _LENTIL_RESO_LNTSAVER_H_

#ifdef __cplusplus
extern "C" {
#endif

//////////////
// Includes //
#include <stdio.h>

#include "../core/errors.h"
#include "model.h"

//////////
// Code //

// Saving a Lentil_Reso_Model* to a given file on disk in the .lnt format, a
// file format made for lentil.
//
// For file spec information, check out the documentation in doc/lntmodel.txt
void Lentil_Reso_saveLntModel(FILE*, Lentil_Reso_Model*, Lentil_Core_Error*);

// Saving a Lentil_Reso_Model* to a given path on disk - opens a file and then
// calls Lentil_Reso_saveLntModel.
void Lentil_Reso_saveLntModelStr(const char*, Lentil_Reso_Model*, Lentil_Core_Error*);

#ifdef __cplusplus
}
#endif

#endif
