#include "modelutils.h"

//////////////
// Includes //
#include <stdlib.h>
#include <stdio.h>

#include "../core/debug.h"
#include "matrixgen.h"

//////////
// Code //

// Filling a VBO and EBO with the information from a single
// Lentil_Reso_Model_Group. Returns the EBO size for use in glDrawArrays.
int Lentil_Rend_fillBuffers(Lentil_Reso_Model* model, int group, GLuint vbo, GLuint ebo, Lentil_Core_Error* pErr) {
    // Finding the size of the VBO and EBO.
    int vboSize = 0,
        eboSize = 0;

    for (int i = 0; i < model->groups[group].facesLength; i++) {
        vboSize += 7 *  model->groups[group].faces[i].triadsLength;
        eboSize +=      model->groups[group].faces[i].triadsLength;
    }

    // Allocating and filling the respective vertices.
    float*        vs = malloc(vboSize * sizeof(float));
    unsigned int* es = malloc(eboSize * sizeof(unsigned int));
    int vi = 0,
        ei = 0;

    Lentil_Reso_Model_Triad triad;
    for (int i = 0; i < model->groups[group].facesLength; i++) {
        for (int j = 0; j < model->groups[group].faces[i].triadsLength; j++) {
            // Filling the VBO data.
            triad = model->groups[group].faces[i].triads[j];

            vs[vi    ] = model->pVertices[triad.pos - 1].x;
            vs[vi + 1] = model->pVertices[triad.pos - 1].y;
            vs[vi + 2] = model->pVertices[triad.pos - 1].z;
            vs[vi + 3] = model->pVertices[triad.pos - 1].w;

            vs[vi + 4] = model->tVertices[triad.tex - 1].x;
            vs[vi + 5] = model->tVertices[triad.tex - 1].y;
            vs[vi + 6] = model->tVertices[triad.tex - 1].w;

            vi += 7;
            es[ei++] = (vi / 7) - 1;
        }
    }

    // Filling the buffers.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, vboSize * sizeof(float), vs, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboSize * sizeof(unsigned int), es, GL_DYNAMIC_DRAW);

    // Freeing data.
    free(vs);
    free(es);

    return eboSize;
}

// Constructing a new Lentil_Rend_ModelRender from a Lentil_Reso_Model.
Lentil_Rend_ModelRender* Lentil_Rend_ModelRender_new(Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    Lentil_Rend_ModelRender* mr = malloc(sizeof(Lentil_Rend_ModelRender));

    mr->cameraMatrix = malloc(9 * sizeof(float));

    mr->count = model->groupsLength;
    mr->vLengths = malloc(mr->count * sizeof(int));

    mr->vaos = malloc(mr->count * sizeof(GLuint));
    mr->vbos = malloc(mr->count * sizeof(GLuint));
    mr->ebos = malloc(mr->count * sizeof(GLuint));

    glGenVertexArrays(mr->count, mr->vaos);
    glGenBuffers(mr->count, mr->vbos);
    glGenBuffers(mr->count, mr->ebos);

    for (int i = 0; i < model->groupsLength; i++) {
        glBindVertexArray(mr->vaos[i]);
        mr->vLengths[i] = Lentil_Rend_fillBuffers(model, i, mr->vbos[i], mr->ebos[i], pErr);

        if (Lentil_Core_isError(*pErr)) {
            if (Lentil_Core_debugLevel(-1) > 0)
                printf("Failed to fill buffers for model index %d.\n", i);

            Lentil_Rend_ModelRender_destroy(mr);

            return NULL;
        }
    }

    return mr;
}

// Destroying the data contained in a Lentil_Rend_ModelRender.
void Lentil_Rend_ModelRender_destroy(Lentil_Rend_ModelRender* mr) {
    glDeleteVertexArrays(mr->count, mr->vaos);
    glDeleteBuffers(mr->count, mr->vbos);
    glDeleteBuffers(mr->count, mr->ebos);

    free(mr->cameraMatrix);
    free(mr->vLengths);
    free(mr->vaos);
    free(mr->vbos);
    free(mr->ebos);
    free(mr);
}

// Performing a render upon the data contained within a Lentil_Rend_ModelRender.
void Lentil_Rend_ModelRender_render(Lentil_Rend_ModelRender* mr, GLuint texture, GLuint shader) {
    // Binding the shader.
    glUseProgram(shader);

    // Binding the texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindFragDataLocation(shader, 0, "out_color");

    // Setting the scale and texture location.
    glUniformMatrix3fv(glGetUniformLocation(shader, "camera_matrix"), 1, false, mr->cameraMatrix);
    glUniform2f(glGetUniformLocation(shader, "window_size"), 640, 480);
    glUniform1f(glGetUniformLocation(shader, "scale"), 0.5);
    glUniform1i(glGetUniformLocation(shader, "tex"), 0);

    // Iterating through the (array) buffer objects and performing renders.
    GLuint pattr, tattr;
    for (int i = 0; i < mr->count; i++) {
        glBindVertexArray(mr->vaos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, mr->vbos[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mr->ebos[i]);

        pattr = glGetAttribLocation(shader, "pvert");
        glEnableVertexAttribArray(pattr);
        glVertexAttribPointer(pattr, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);

        tattr = glGetAttribLocation(shader, "tvert");
        glEnableVertexAttribArray(tattr);
        glVertexAttribPointer(tattr, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));

        glDrawElements(
            GL_TRIANGLE_STRIP,
            mr->vLengths[i],
            GL_UNSIGNED_INT,
            (void*)0
        );
    }
}

// Updating the rotation and then performing a render.
void Lentil_Rend_ModelRender_renderRot(Lentil_Rend_ModelRender* mr, GLuint texture, GLuint shader, float xrot, float yrot) {
    Lentil_Rend_fillMatrix(mr->cameraMatrix, xrot, yrot);
    Lentil_Rend_ModelRender_render(mr, texture, shader);
}
