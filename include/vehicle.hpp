#pragma once
// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "model.hpp"
#include "entity.hpp"

class Vehicle:Entity{

    public:
        Vehicle(Model object,glm::vec4 initialPosition,
            GLfloat angleX,GLfloat angleY,GLfloat angleZ,
            GLfloat sx, GLfloat sy, GLfloat sz );

        int move();

    private:
        glm::vec4 vehiclePosition;
        glm::vec4 resultantForce;
};