#pragma once
// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


class Vehicle{

    public:
        Vehicle(glm::vec4 position);

        int move();

    private:
        glm::vec4 vehiclePosition;
        glm::vec4 resultantForce;
};