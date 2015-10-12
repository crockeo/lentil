#ifndef _LENTIL_RESO_MODEL_H_
#define _LENTIL_RESO_MODEL_H_

//////////////
// Includes //
#include <stdbool.h>

#include "../core/errors.h"

//////////
// Code //

// Information to represent a single face on a 
typedef struct {
    int vertex;
    int textureVertex;
    int normalVertex;
} Lentil_Reso_Face;

// Information to represent a model resource.
typedef struct {
    int posVerticesSize;
    int posVerticesNum;
    float* posVertices;

    int texVerticesSize;
    int texVerticesNum;
    float* texVertices;

    int norVerticesSize;
    int norVerticesNum;
    float* norVertices;

    int facesSize;
    int facesNum;
    Lentil_Reso_Face* faces;
} Lentil_Reso_Model;

// Constructing a new Lentil_Reso_Model on the heap.
Lentil_Reso_Model* Lentil_Reso_createModel();

// Freeing up all of the memory from a given model.
void Lentil_Reso_destroyModel(Lentil_Reso_Model*);

// Adding new values into a model.
void Lentil_Reso_addPosVertex(Lentil_Reso_Model*, float, float, float, float);
void Lentil_Reso_addTexVertex(Lentil_Reso_Model*, float, float, float);
void Lentil_Reso_addNorVertex(Lentil_Reso_Model*, float, float, float);
void Lentil_Reso_addFace(Lentil_Reso_Model*, Lentil_Reso_Face);

#endif
