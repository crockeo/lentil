#include "texture.h"

//////////////
// Includes //
#include <stdlib.h>
#include <stdio.h>
#include <png.h>

#include "../core/debug.h"

//////////
// Code //

#define PNG_HEADER_SIZE 8

// Printing out a texture of a given width and height.
void Lentil_Reso_printTexture(GLuint tex, bool alpha, int width, int height) {
    png_byte* data = malloc(width * height * (alpha ? 4 : 3) * sizeof(png_byte));

    glBindTexture(GL_TEXTURE_2D, tex);
    glGetTexImage(
        GL_TEXTURE_2D,
        0,
        alpha ? GL_RGBA : GL_RGB,
        GL_UNSIGNED_BYTE,
        data
    );

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int i = row * width + col;
            if (alpha)
                printf("(%u %u %u %u) ", data[i], data[i + 1], data[i + 2], data[i + 3]);
            else
                printf("(%u %u %u) ", data[i], data[i + 1], data[i + 2]);
        }

        printf("\n");
    }

    free(data);
}

// Populating a given texture (represented as a GLuint) with some PNG data
// (represented by a FILE pointer).
void Lentil_Reso_populateTexture(FILE* texFile, GLuint texture, Lentil_Core_Error* pErr) {
    // Checking the validity of a given PNG file.
    png_byte* header = malloc(PNG_HEADER_SIZE * sizeof(png_byte));
    fread(header, 1, PNG_HEADER_SIZE, texFile);

    bool valid = png_sig_cmp(header, 0, PNG_HEADER_SIZE) == 0;
    free(header);

    if (!valid) {
        pErr->code = Lentil_Core_PNGLOADFAILED;

        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Invalid PNG header.\n");

        return;
    }

    // Creating the prerequisite PNG structs.
    png_structp pngPtr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        NULL,
        NULL
    );

    if (pngPtr == NULL) {
        pErr->code = Lentil_Core_PNGLOADFAILED;

        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Could not create png_structp.\n");

        return;
    }

    png_infop infoPtr = png_create_info_struct(pngPtr);
    if (infoPtr == NULL) {
        pErr->code = Lentil_Core_PNGLOADFAILED;
        png_destroy_read_struct(&pngPtr, &infoPtr, NULL);

        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Could not create either the start or end info structs.\n");

        return;
    }

    // Setting up libpng.
    if (setjmp(png_jmpbuf(pngPtr))) {
        pErr->code = Lentil_Core_PNGLOADFAILED;
        png_destroy_read_struct(&pngPtr, &infoPtr, NULL);

        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Unknown libpng error.\n");

        return;
    }

    png_init_io(pngPtr, texFile);
    png_set_sig_bytes(pngPtr, PNG_HEADER_SIZE);

    // Loading info data.
    png_uint_32 width, height;
    int bitDepth, colorType;

    png_read_info(pngPtr, infoPtr);
    png_get_IHDR(
        pngPtr,
        infoPtr,
        &width,
        &height,
        &bitDepth,
        &colorType,
        NULL,
        NULL,
        NULL
    );

    // Allocating space for the png.
    int rowBytes = png_get_rowbytes(pngPtr, infoPtr);
    png_byte* imgData = malloc(rowBytes * height * sizeof(png_byte));
    png_bytep* rowPointers = malloc(height * sizeof(png_bytep));

    if (imgData == NULL || rowPointers == NULL) {
        png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
        if (imgData != NULL)
            free(imgData);
        if (rowPointers != NULL)
            free(rowPointers);

        pErr->code = Lentil_Core_PNGLOADFAILED;

        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Could not allocate image data.\n");

        return;
    }

    // Starting to load png data.
    for (int i = 0; i < height; i++)
        rowPointers[height - 1 - i] = imgData + i * rowBytes;
    png_read_image(pngPtr, rowPointers);

    // Getting the color type of the png.
    bool alpha = colorType == PNG_COLOR_TYPE_RGB_ALPHA;

    // Filling the texture with actual image data.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        alpha ? GL_RGBA8 : GL_RGB8,
        width,
        height,
        0,
        alpha ? GL_RGBA : GL_RGB,
        GL_UNSIGNED_BYTE,
        imgData
    );

    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        pErr->code = Lentil_Core_PNGLOADFAILED;

        if (Lentil_Core_debugLevel(-1) > 0)
            printf("OpenGL error: %d\n", err);
    }

    // Cleaning up and returning.
    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
    free(imgData);
    free(rowPointers);
}

// Attempting to load a texture from a given location on disk.
//
// The texture must be in a .png format.
GLuint Lentil_Reso_loadTexture(const char* path, Lentil_Core_Error* pErr) {
    // Trying to open the texture file.
    FILE* texFile = fopen(path, "r");
    if (texFile == NULL) {
        pErr->code = Lentil_Core_FILENOTFOUND;

        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Failed to open texture: %s.\n", path);

        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);

    Lentil_Core_Error err = Lentil_Core_defaultError();
    Lentil_Reso_populateTexture(texFile, texture, &err);

    fclose(texFile);

    if (Lentil_Core_isError(err)) {
        pErr->code = err.code;
        glDeleteTextures(1, &texture);
        return 0;
    }

    fclose(texFile);

    return 0;
}
