#pragma once
// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "model.hpp"
#include "entity.hpp"
#define DEFAULT_FRONT_VECTOR glm::vec4(0.85,0.0,0.5,0.0)
#define DEFAULT_VEHICLE_ACCELERATION 10.0
enum CarGear{
    normal,
    reverse
};
class Vehicle: public Entity{

    public:
        Vehicle(Model object);

        Vehicle(Model object,glm::vec4 initialPosition,
            GLfloat angleX,GLfloat angleY,GLfloat angleZ,
            GLfloat sx, GLfloat sy, GLfloat sz );

        int move(CarGear gear);

    private:
        float acceleration;
        glm::vec4 vehiclePosition;
        glm::vec4 resultantForce;
        glm::vec4 frontVector;
};