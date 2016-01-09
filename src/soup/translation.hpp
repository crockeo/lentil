#ifndef _TRANSLATION_HPP_
#define _TRANSLATION_HPP_

//////////////
// Includes //
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

//////////
// Code //

// A vector to be used for user-interaction based translation.
class TransVec {
private:
    glm::vec3 accelSpeed;
    glm::vec3 maxSpeed;
    glm::vec3 minSpeed;
    glm::vec3 speed;
    glm::vec3 vec;
    bool instant;

    int px, nx,
        py, ny,
        pz, nz;

public:
    // Constructing a translation vector with axis-specific values.
    TransVec(glm::vec3, glm::vec3, glm::vec3, bool, int, int, int, int, int, int);

    // Constructing a translation vector with axis-equivalent values.
    TransVec(double, double, double, bool, int, int, int, int, int, int);

    // Updating the TransVec with an explicit set of angles to affect
    // translation.
    void update(GLFWwindow*, double, glm::vec3);

    // Updating the TransVec with a given GLFWwindow and delta time.
    void update(GLFWwindow*, double);

    // Getting the vector.
    glm::vec3 getVec() const;
};

#endif
