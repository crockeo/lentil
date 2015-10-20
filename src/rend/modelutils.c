#include "modelutils.h"

//////////////
// Includes //
#include <stdlib.h>

#include <stdio.h>
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
        eboSize += 3 * (model->groups[group].faces[i].triadsLength - 2);
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

            // Filling the EBO data.
            if (j < model->groups[group].faces[i].triadsLength - 2) {
                int p = (vi / 7) - 1;

                es[ei    ] = p;
                es[ei + 1] = p + 1;
                es[ei + 2] = p + 2;

                ei += 3;
            }
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

// Rendering a model with a given texture and a given shader.
void Lentil_Rend_renderModel(Lentil_Reso_Model* model, GLuint texture, GLuint shader, Lentil_Core_Error* pErr) {
    // Creating the VAO, VBO, and EBO.
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // Binding the shader.
    glUseProgram(shader);

    // Binding the texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader, "tex"), 0);
    glBindFragDataLocation(shader, 0, "out_color");

    // Binding the positional and vertex positions.
    GLuint pattr, tattr;
    pattr = glGetAttribLocation(shader, "pvert");
    glEnableVertexAttribArray(pattr);
    glVertexAttribPointer(pattr, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);

    tattr = glGetAttribLocation(shader, "tvert");
    glEnableVertexAttribArray(tattr);
    glVertexAttribPointer(tattr, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(GL_FLOAT)));

    // Drawing each of the groups.
    int count;
    for (int i = 0; i < model->groupsLength; i++) {
        count = Lentil_Rend_fillBuffers(model, i, vbo, ebo, pErr);
        if (Lentil_Core_isError(*pErr))
            return;

        glDrawElements(
            GL_TRIANGLES,
            count,
            GL_UNSIGNED_INT,
            (void*)0
        );
    }

    // Cleaning up the VAO, VBO, and EBO.
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
