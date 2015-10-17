#include "model.h"

//////////////
// Includes //
#include "../core/array.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//////////
// Code //

// Creating and returning a new model.
Lentil_Reso_Model* Lentil_Reso_Model_new() {
    Lentil_Reso_Model* model = malloc(sizeof(Lentil_Reso_Model));

    // Initializing all of the vertices.
    Lentil_Core_initArray(model->pVertices, Lentil_Reso_Model_PVertex);
    Lentil_Core_initArray(model->tVertices, Lentil_Reso_Model_TVertex);
    Lentil_Core_initArray(model->nVertices, Lentil_Reso_Model_NVertex);
    Lentil_Core_initArray(model->groups, Lentil_Reso_Model_Group);

    // Initializing the material.
    model->material = NULL;

    return model;
}

// Destroying a model and its associated resources.
void Lentil_Reso_Model_destroy(Lentil_Reso_Model* model) {
    // Freeing all of the vertices.
    free(model->pVertices);
    free(model->tVertices);
    free(model->nVertices);

    // Freeing the memory associated with the groups.
    for (int i = 0; i < model->groupsLength; i++) {
        for (int j = 0; j < model->groups[i].facesLength; j++)
            free(model->groups[i].faces[j].triads);

        free(model->groups[i].faces);
    }
    free(model->groups);

    // Freeing the material if this model owns it.
    if (model->ownsMaterial && model->material != NULL)
        free(model->material);

    // Freeing the pointer to the model itself.
    free(model);
}
