#include "model.h"

//////////////
// Includes //
#include <stdlib.h>

#include "../core/debug.h"

//////////
// Code //

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
