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

    // Generating a single direction given two keys, one for positive input, and
    // one for negative input. Also requires reference to the current speed for
    // speedy deceleration.
    float direction(GLFWwindow*, float, float, float, int, int);

    // Generating a direction vector given the current input states.
    glm::vec3 directionVector(GLFWwindow*);

    // Constructing a rotational matrix for the translation vector.
    glm::mat3 rotationalMatrix(glm::vec3);

    // Capping a single speed axis.
    void capAxis(float, float, float&, float&);

    // Capping the speed given a reference to a direction matrix (which is used
    // to control acceleration (or lack thereof) at low speeds).
    void capSpeeds(glm::vec3&);

public:
    // Constructing a translation vector with axis-specific values.
    TransVec(glm::vec3, glm::vec3, glm::vec3, bool, int, int, int, int, int, int);

    // Constructing a translation vector with axis-equivalent values.
    TransVec(float, float, float, bool, int, int, int, int, int, int);

    // Updating the TransVec with an explicit set of angles to affect
    // translation.
    void update(GLFWwindow*, float, glm::vec3);

    // Updating the TransVec with a given GLFWwindow and delta time.
    void update(GLFWwindow*, float);

    // Getting the vector.
    glm::vec3 getVec() const;
};

#endif
