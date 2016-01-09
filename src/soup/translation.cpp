#include "translation.hpp"

//////////////
// Includes //
#include <iostream>
#include <math.h>

//////////
// Code //

std::ostream& operator<<(std::ostream& o, glm::vec3 vec) {
    return o << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

////
// TransVec

// Generating a single direction given two keys, one for positive input, and
// one for negative input. Also requires reference to the current speed for
// speedy deceleration.
float TransVec::direction(GLFWwindow* window, float maxSpeed, float minSpeed, float speed, int pk, int nk) {
    bool p = glfwGetKey(window, pk) == GLFW_PRESS,
         n = glfwGetKey(window, nk) == GLFW_PRESS;

    if (p && n) { }
    else if (p) {
        if (speed > maxSpeed)
            return 0.0;
        if (speed < minSpeed)
            return 1.3;
        return 1.0;
    } else if (n) {
        if (speed < -maxSpeed)
            return 0.0;
        if (speed > -minSpeed)
            return -1.3;
        return -1.0;
    }

    if (speed >= -minSpeed && speed <= minSpeed)
         return 0.0;
    return speed > 0 ? -1.0 : 1.0;
}

// Generating a direction vector given the current input states.
glm::vec3 TransVec::directionVector(GLFWwindow* window) {
    return glm::vec3(direction(window, maxSpeed.x, minSpeed.x, speed.x, px, nx),
                     direction(window, maxSpeed.y, minSpeed.y, speed.y, py, ny),
                     direction(window, maxSpeed.z, minSpeed.z, speed.z, pz, nz));
}

// Constructing a rotational matrix for the translation vector.
glm::mat3 TransVec::rotationalMatrix(glm::vec3 rotation) {
    glm::mat3 xrot(
        1, 0,               0,
        0, cos(rotation.x), -sin(rotation.x),
        0, sin(rotation.x), cos(rotation.x)
    );

    glm::mat3 yrot(
        cos(rotation.y),  0, sin(rotation.y),
        0,                1, 0,
        -sin(rotation.y), 0, cos(rotation.y)
    );

    glm::mat3 zrot(
        cos(rotation.z), -sin(rotation.z), 0,
        sin(rotation.z), cos(rotation.z),  0,
        0,               0,                1
    );

    return glm::matrixCompMult(zrot, glm::matrixCompMult(yrot, xrot));
}

// Capping a single speed axis.
void TransVec::capAxis(float maxSpeed, float minSpeed, float& speed, float& direction) {
    if (speed < -maxSpeed) {
        speed = -maxSpeed;
        if (direction < 0)
            direction = 0;
    }

    if (speed > maxSpeed) {
        speed = maxSpeed;
        if (direction > 0)
            direction = 0;
    }

    if (speed >= -minSpeed && speed <= minSpeed) {
        if ((speed > 0 && !(direction > 0)) ||
            (speed < 0 && !(direction < 0))) {
            direction = 0;
            speed = 0;
        }
    }
}

// Capping the speed given a reference to a direction matrix (which is used
// to control acceleration (or lack thereof) at low speeds).
void TransVec::capSpeeds(glm::vec3& direction) {
    capAxis(maxSpeed.x, minSpeed.x, speed.x, direction.x);
    capAxis(maxSpeed.y, minSpeed.y, speed.y, direction.y);
    capAxis(maxSpeed.z, minSpeed.z, speed.z, direction.z);
}

// Constructing a translation vector with axis-specific values.
TransVec::TransVec(glm::vec3 accelSpeed,
                   glm::vec3 maxSpeed, glm::vec3 minSpeed,
                   bool instant,
                   int px, int nx,
                   int py, int ny,
                   int pz, int nz) :
        accelSpeed(accelSpeed),
        maxSpeed(maxSpeed), minSpeed(minSpeed),
        instant(instant),
        px(px), nx(nx),
        py(py), ny(ny),
        pz(pz), nz(nz) { }

// Constructing a translation vector with axis-equivalent values.
TransVec::TransVec(float accelSpeed,
                   float maxSpeed, float minSpeed,
                   bool instant,
                   int px, int nx,
                   int py, int ny,
                   int pz, int nz) :
        TransVec(glm::vec3(accelSpeed),
                 glm::vec3(maxSpeed), glm::vec3(minSpeed),
                 instant,
                 px, nx,
                 py, ny,
                 pz, nz) { }

// Updating the TransVec with an explicit set of angles to affect
// translation.
void TransVec::update(GLFWwindow* window, float dt, glm::vec3 rot) {
    glm::vec3 direction = directionVector(window);
    glm::mat3 rotation = rotationalMatrix(rot);

    direction = rotation * direction;
    if (instant)
        vec += direction * maxSpeed * dt;
    else {
        capSpeeds(direction);
        speed += accelSpeed * direction * dt;
        vec += speed * dt;
    }
}

// Updating the TransVec with a given GLFWwindow and delta time.
void TransVec::update(GLFWwindow* window, float dt) {
    update(window, dt, glm::vec3(0));
}

// Getting the vector.
glm::vec3 TransVec::getVec() const { return vec; }
