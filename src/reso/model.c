#include "model.h"

//////////////
// Includes //
#include <stdlib.h>

//////////
// Code //

// Creating and returning a new model.
Lentil_Reso_Model* Lentil_Reso_Model_new() {
    Lentil_Reso_Model* model = malloc(sizeof(Lentil_Reso_Model));

    // Initializing all of the vertices.
    model->pVerticesSize = 1;
    model->pVerticesLength = 0;
    model->pVertices = malloc(model->pVerticesSize * sizeof(float));

    model->tVerticesSize = 1;
    model->tVerticesLength = 0;
    model->tVertices = malloc(model->tVerticesSize * sizeof(float));

    model->nVerticesSize = 1;
    model->nVerticesLength = 0;
    model->nVertices = malloc(model->nVerticesSize * sizeof(float));

    // Initializing the groups.
    model->groupsSize = 1;
    model->groupsLength = 0;
    model->groups = malloc(model->groupsSize * sizeof(Lentil_Reso_Model_Group));

    // Initializing the material.
    model->material = NULL;

    return model;
}

// Destroying a model and its associated resources.
void Lentil_Reso_Model_destroy(Lentil_Reso_Model* model) {
    free(model->pVertices);
    free(model->tVertices);
    free(model->nVertices);

    free(model->groups);

    if (model->material != NULL)
        free(model->material);

    free(model);
}
