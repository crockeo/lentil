#ifndef _LENTIL_CORE_ARRAY_H_
#define _LENTIL_CORE_ARRAY_H_

// WARNING: This is probably really bad practice. That being said, it's really
// useful.
//
// - array : needs to be the name of some value that is a pointer, and has the
//   definitions of Length and Size as ints under the name of those strings
//   appended to the variable defined by array.
//
// - value : needs to be the name of the value that you're trying to append to
//   the array.
//
// - type : needs to be name that represents the type of the value you're trying
//   to add. For instance if you're trying to add a Lentil_Reso_Model_Face, then
//   type needs to be Lentil_Reso_Model_Face.
#define Lentil_Core_addElement(array, value, type) \
    if (array ## Length >= array ## Size) { \
        type* narr = malloc(array ## Size * 2 * sizeof(type)); \
        for (int i = 0; i < array ## Length; i++) \
            narr[i] = array[i]; \
\
        free(array); \
        array ## Size *= 2; \
        array = narr; } \
    array[array ## Length++] = value;

#endif
