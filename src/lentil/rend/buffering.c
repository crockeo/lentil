#include "buffering.h"

//////////////
// Includes //
#include <stdlib.h>

//////////
// Code //

/*
    // Filling the buffers.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);


    // Freeing data.
    free(vs);
    free(es);

    return eboSize;
}
*/

// Using a Lentil_Rend_Model to fill a VAO's VBO and EBO with the appropriate
// vertices. The choice of buffer use is defined by the GLenum.
//
// This function returns the vertex count.
int Lentil_Rend_fillBuffers(Lentil_Reso_Model* model, int group, GLuint vao, GLuint vbo, GLuint ebo, GLenum type, Lentil_Core_Error* pErr) {
    int vboSize = 0,
        eboSize = 0;

    for (int i = 0; i < model->groups[group].facesLength; i++) {
        vboSize += 7 *  model->groups[group].faces[i].triadsLength;
        eboSize += 3 * (model->groups[group].faces[i].triadsLength - 1);
    }

    // Allocating space for the vertices and order.
    float*        vs = malloc(vboSize * sizeof(float));
    unsigned int* es = malloc(eboSize * sizeof(unsigned int));
    int vi = 0,
        ei = 0;

    // Filling the vertex data.
    Lentil_Reso_Model_Triad triad;
    for (int i = 0; i < model->groups[group].facesLength; i++) {
        for (int j = 0; j < model->groups[group].faces[i].triadsLength; j++) {
            triad = model->groups[group].faces[i].triads[j];

            // VBO.
            vs[vi    ] = model->pVertices[triad.pos - 1].x;
            vs[vi + 1] = model->pVertices[triad.pos - 1].y;
            vs[vi + 2] = model->pVertices[triad.pos - 1].z;
            vs[vi + 3] = model->pVertices[triad.pos - 1].w;

            vs[vi + 4] = model->tVertices[triad.tex - 1].x;
            vs[vi + 5] = model->tVertices[triad.tex - 1].y;
            vs[vi + 6] = model->tVertices[triad.tex - 1].w;

            vi += 7;

            // EBO.
            if (j < model->groups[group].faces[i].triadsLength - 2) {
                es[ei    ] = (vi / 7) - 1;
                es[ei + 1] = (vi / 7);
                es[ei + 2] = (vi / 7) + 1;

                ei += 3;
            }
        }
    }

    // Filling the OpenGL buffers.
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER        , vi * sizeof(float)       , vs, type);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ei * sizeof(unsigned int), es, type);

    // Cleaning up data.
    free(vs);
    free(es);

    return ei;
}
