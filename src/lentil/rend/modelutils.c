#include "modelutils.h"

//////////////
// Includes //
#include <stdlib.h>
#include <stdio.h>

#include "../core/debug.h"
#include "buffering.h"

//////////
// Code //

// Constructing a new Lentil_Rend_ModelRender from a Lentil_Reso_Model.
Lentil_Rend_ModelRender* Lentil_Rend_ModelRender_new(Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    Lentil_Rend_ModelRender* mr = malloc(sizeof(Lentil_Rend_ModelRender));

    mr->camera = Lentil_Rend_Camera_new();

    mr->count = model->groupsLength;
    mr->vLengths = malloc(mr->count * sizeof(int));

    mr->vaos = malloc(mr->count * sizeof(GLuint));
    mr->vbos = malloc(mr->count * sizeof(GLuint));
    mr->ebos = malloc(mr->count * sizeof(GLuint));

    glGenVertexArrays(mr->count, mr->vaos);
    glGenBuffers(mr->count, mr->vbos);
    glGenBuffers(mr->count, mr->ebos);

    for (int i = 0; i < model->groupsLength; i++) {
        mr->vLengths[i] = Lentil_Rend_fillBuffers(model, i, mr->vaos[i], mr->vbos[i], mr->ebos[i], GL_STATIC_DRAW, pErr);

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
    Lentil_Rend_Camera_destroy(mr->camera);

    glDeleteVertexArrays(mr->count, mr->vaos);
    glDeleteBuffers(mr->count, mr->vbos);
    glDeleteBuffers(mr->count, mr->ebos);

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
    glUniform3f(glGetUniformLocation(shader, "camera_position"), mr->camera->x, mr->camera->y, mr->camera->z);
    glUniform2f(glGetUniformLocation(shader, "camera_rotation"), mr->camera->xrot, mr->camera->yrot);
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
            GL_TRIANGLES,
            mr->vLengths[i],
            GL_UNSIGNED_INT,
            (void*)0
        );
    }
}
