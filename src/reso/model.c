#include "model.h"

//////////////
// Includes //
#include <stdlib.h>

#include "../core/debug.h"

//////////
// Code //

// Expanding the array of vertices to a new size.
void Lentil_Reso_Model_expandVertices(float** vertices, int width, int size) {
    float* nVertices = malloc(width * size * 2 * sizeof(float));
    for (int i = 0; i < size * width; i++)
        nVertices[i] = (*vertices)[i];
    free(*vertices);
    *vertices = nVertices;
}

// Constructing a new Lentil_Reso_Model on the heap.
Lentil_Reso_Model* Lentil_Reso_createModel() {
    Lentil_Reso_Model* model = malloc(sizeof(Lentil_Reso_Model));

    model->posVerticesSize = 1;
    model->posVerticesNum = 0;
    model->posVertices = malloc(model->posVerticesSize * 4 * sizeof(float));;

    model->texVerticesSize = 1;
    model->texVerticesNum = 0;
    model->texVertices = malloc(model->texVerticesSize * 3 * sizeof(float));

    model->norVerticesSize = 1;
    model->norVerticesNum = 0;
    model->norVertices = malloc(model->norVerticesSize * 3 * sizeof(float));

    model->facesSize = 1;
    model->facesNum = 0;
    model->faces = malloc(model->facesSize * sizeof(Lentil_Reso_Face));

    return model;
}

// Freeing up all of the memory from a given model.
void Lentil_Reso_destroyModel(Lentil_Reso_Model* model) {
    free(model->posVertices);
    free(model->texVertices);
    free(model->norVertices);
    free(model->faces);
}

// Adding new values into a model.
void Lentil_Reso_addPosVertex(Lentil_Reso_Model* model, float x, float y, float z, float w) {
    if (model->posVerticesNum >= model->posVerticesSize) {
        Lentil_Reso_Model_expandVertices(&model->posVertices, 4, model->posVerticesSize);
        model->posVerticesSize *= 2;
    }

    float* location = model->posVertices + (4 * model->posVerticesNum);
    location[0] = x;
    location[1] = y;
    location[2] = z;
    location[3] = w;

    model->posVerticesNum++;
}

void Lentil_Reso_addTexVertex(Lentil_Reso_Model* model, float x, float y, float w) {
    if (model->texVerticesNum >= model->texVerticesSize) {
        Lentil_Reso_Model_expandVertices(&model->texVertices, 3, model->texVerticesSize);
        model->texVerticesSize *= 2;
    }

    float* location = model->texVertices + (3 * model->texVerticesNum);
    location[0] = x;
    location[1] = y;
    location[2] = w;

    model->texVerticesNum++;
}

void Lentil_Reso_addNorVertex(Lentil_Reso_Model* model, float x, float y, float z) {
    if (model->norVerticesNum >= model->norVerticesSize) {
        Lentil_Reso_Model_expandVertices(&model->norVertices, 3, model->norVerticesSize);
        model->norVerticesSize *= 2;
    }

    float* location = model->norVertices + (3 * model->norVerticesNum);
    location[0] = x;
    location[1] = y;
    location[2] = z;

    model->norVerticesNum++;
}

void Lentil_Reso_addFace(Lentil_Reso_Model* model, Lentil_Reso_Face face) {
    if (model->facesNum >= model->facesSize) {
        Lentil_Reso_Face* nFaces = malloc(model->facesSize * 2 * sizeof(Lentil_Reso_Face));
        for (int i = 0; i < model->facesSize; i++)
            nFaces[i] = model->faces[i];
        free(model->faces);
        model->faces = nFaces;
        model->facesSize *= 2;
    }

    model->faces[model->facesNum] = face;
    model->facesNum++;
}
