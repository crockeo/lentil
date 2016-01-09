#include "translation.hpp"

//////////
// Code //

// Constructing a translation vector with axis-specific values.
TransVec::TransVec(glm::vec3 accelSpeed,
                   glm::vec3 maxSpeed, glm::vec3 minSpeed,
                   bool instant,
                   int px, int nx,
                   int py, int ny,
                   int pz, int nz) {

}

// Constructing a translation vector with axis-equivalent values.
TransVec::TransVec(double accelSpeed,
                   double maxSpeed, double minSpeed,
                   bool instant,
                   int px, int nx,
                   int py, int ny,
                   int pz, int nz) :
        TransVec(glm::vec3(accelSpeed),
                 glm::vec3(maxSpeed), glm::vec3(minSpeed),
                 instant,
                 px, nx,
                 py, ny,
                 pz, ny) { }

// Updating the TransVec with an explicit set of angles to affect
// translation.
void TransVec::update(GLFWwindow* window, double dt, glm::vec3 rot) {
    // TODO: Actually perform the update?
}

// Updating the TransVec with a given GLFWwindow and delta time.
void TransVec::update(GLFWwindow* window, double dt) {
    update(window, dt, glm::vec3(0));
}

// Getting the vector.
glm::vec3 TransVec::getVec() const { return vec; }
