#ifndef _LENTIL_RESO_MODEL_H_
#define _LENTIL_RESO_MODEL_H_

//////////////
// Includes //
#include "material.h"

//////////
// Code //

// A single positional vertex.
typedef struct {
    float x, y, z, w;
} Lentil_Reso_Model_PosVertex;

// A single texture vertex.
typedef struct {
    float x, y, w;
} Lentil_Reso_Model_TexVertex;

// A single normal vertex.
typedef struct {
    float x, y, z;
} Lentil_Reso_Model_NorVertex;

// A triad of positional, texture, and normal vertex to be used in face
// definitions.
typedef struct {
    int pos, tex, nor;
} Lentil_Reso_Model_Triad;

// A single face of 3 or more Lentil_Reso_Model_Triads.
typedef struct {
    int length;
    int size;
    Lentil_Reso_Model_Triad* triads;
} Lentil_Reso_Model_Face;

// A group of faces with a common material.
typedef struct {
    const char* name;
    const char* material;
    Lentil_Reso_Model_Face* faces;
} Lentil_Reso_Model_Group;

// An entire model composed 
typedef struct {
    int pVerticesSize, pVerticesLength;
    Lentil_Reso_Model_PosVertex* pVertices;

    int tVerticesSize, tVerticesLength;
    Lentil_Reso_Model_TexVertex* tVertices;

    int nVerticesSize, nVerticesLength;
    Lentil_Reso_Model_NorVertex* nVertices;

    int groupsSize, groupsLength;
    Lentil_Reso_Model_Group* groups;

    Lentil_Reso_Material* material;
} Lentil_Reso_Model;

// Creating and returning a new model.
Lentil_Reso_Model* Lentil_Reso_Model_new();

// Destroying a model and its associated resources.
void Lentil_Reso_Model_destroy(Lentil_Reso_Model*);

#endif
