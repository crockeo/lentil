#include "model.h"

//////////////
// Includes //
#include <stdlib.h>
#include <string.h>

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
    // Freeing all of the vertices.
    free(model->pVertices);
    free(model->tVertices);
    free(model->nVertices);

    // Freeing the memory associated with the groups.
    for (int i = 0; i < model->groupsLength; i++) {
        for (int j = 0; j < model->groups[i].facesLength; j++)
            free(model->groups[j].faces[j].triads);
        free(model->groups[i].faces);
    }
    free(model->groups);

    // Freeing the material if this model owns it.
    if (model->ownsMaterial && model->material != NULL)
        free(model->material);

    // Freeing the pointer to the model itself.
    free(model);
}

// Adding vertices.
void Lentil_Reso_Model_addPVertex(Lentil_Reso_Model* model, Lentil_Reso_Model_PVertex vertex) {
    // Expanding the array if necessary.
    if (model->pVerticesLength >= model->pVerticesSize) {
        Lentil_Reso_Model_PVertex* nPVertices = malloc(model->pVerticesSize * 2 * sizeof(Lentil_Reso_Model_PVertex));
        for (int i = 0; i < model->pVerticesLength; i++)
            nPVertices[i] = model->pVertices[i];

        free(model->pVertices);
        model->pVerticesSize *= 2;
        model->pVertices = nPVertices;
    }

    // Adding the vertex.
    model->pVertices[model->pVerticesLength++] = vertex;
}

void Lentil_Reso_Model_addTVertex(Lentil_Reso_Model* model, Lentil_Reso_Model_TVertex vertex) {
    // Expanding the array if necessary.
    if (model->tVerticesLength >= model->tVerticesSize) {
        Lentil_Reso_Model_TVertex* nPVertices = malloc(model->tVerticesSize * 2 * sizeof(Lentil_Reso_Model_TVertex));
        for (int i = 0; i < model->tVerticesLength; i++)
            nPVertices[i] = model->tVertices[i];

        free(model->tVertices);
        model->tVerticesSize *= 2;
        model->tVertices = nPVertices;
    }

    // Adding the vertex.
    model->tVertices[model->tVerticesLength++] = vertex;
}

void Lentil_Reso_Model_addNVertex(Lentil_Reso_Model* model, Lentil_Reso_Model_NVertex vertex) {
    // Expanding the array if necessary.
    if (model->nVerticesLength >= model->nVerticesSize) {
        Lentil_Reso_Model_NVertex* nPVertices = malloc(model->nVerticesSize * 2 * sizeof(Lentil_Reso_Model_NVertex));
        for (int i = 0; i < model->nVerticesLength; i++)
            nPVertices[i] = model->nVertices[i];

        free(model->nVertices);
        model->nVerticesSize *= 2;
        model->nVertices = nPVertices;
    }

    // Adding the vertex.
    model->nVertices[model->nVerticesLength++] = vertex;
}

// Adding a new group.
void Lentil_Reso_Model_addGroup(Lentil_Reso_Model* model, const char* name) {
    // Expanding the array if necessary.
    if (model->groupsLength >= model->groupsSize) {
        Lentil_Reso_Model_Group* nGroups = malloc(model->groupsSize * 2 * sizeof(Lentil_Reso_Model_Group));
        for (int i = 0; i < model->groupsLength; i++)
            nGroups[i] = model->groups[i];

        free(model->groups);
        model->groupsSize *= 2;
        model->groups = nGroups;
    }

    // Adding the group.
    Lentil_Reso_Model_Group group;

    int slen = strlen(name);
    group.name = malloc((slen + 1) * sizeof(char));
    strncpy(group.name, name, slen);
    group.name[slen] = '\0';

    group.material = "";

    group.facesSize = 1;
    group.facesLength = 0;
    group.faces = malloc(group.facesSize * sizeof(Lentil_Reso_Model_Face));

    model->groups[model->groupsLength++] = group;
}

// Setting the material of a Lentil_Reso_Model.
void Lentil_Reso_Model_setMaterial(Lentil_Reso_Model* model, Lentil_Reso_Material* material, bool ownsMaterial) {
    model->ownsMaterial = ownsMaterial;
    model->material = material;
}
