#include "modelutils.h"

//////////////
// Includes //
#include <stdlib.h>

//////////
// Code //

// Filling a VBO with the information from a model.
void Lentil_Rend_fillVBO(Lentil_Reso_Model* model, GLuint vbo, Lentil_Core_Error* pErr) {
    int vboSize = 0;
    float* vs = malloc(vboSize * sizeof(float));

    // TODO: Fill vs

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vboSize * sizeof(float), vs, GL_DYNAMIC_DRAW);

    free(vs);
}

// Filling an EBO with the information from a model. It returns the number of
// vertices within the EBO.
int Lentil_Rend_fillEBO(Lentil_Reso_Model* model, GLuint ebo, Lentil_Core_Error* pErr) {
    int eboSize = 0;
    unsigned int* vs = malloc(eboSize * sizeof(unsigned int));

    // TODO: Fill vs

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboSize * sizeof(unsigned int), vs, GL_DYNAMIC_DRAW);

    free(vs);
    
    return eboSize;
}

// Rendering a model with a given texture and a given shader.
void Lentil_Rend_renderModel(Lentil_Reso_Model* model, GLuint texture, GLuint shader, Lentil_Core_Error* pErr) {
    // Creating and filling the VAO, VBO, and EBO.
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    Lentil_Rend_fillVBO(model, vbo, pErr);
    int count = Lentil_Rend_fillEBO(model, ebo, pErr);
    if (Lentil_Core_isError(*pErr))
        return;

    // Binding the shader and the texture.
    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindFragDataLocation(shader, 0, "out_color");

    // Binding the positional and vertex positions.
    GLuint pattr, tattr;
    pattr = glGetAttribLocation(shader, "pvert");
    glEnableVertexAttribArray(pattr);
    glVertexAttribPointer(pattr, 3, GL_FLOAT, false, 5 * 2, 0);

    tattr = glGetAttribLocation(shader, "tvert");
    glEnableVertexAttribArray(tattr);
    glVertexAttribPointer(tattr, 2, GL_FLOAT, false, 5 * 2, (void*)(2 * sizeof(GL_FLOAT)));

    // Drawing the element.
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);

    // Cleaning up the VAO, VBO, and EBO.
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
