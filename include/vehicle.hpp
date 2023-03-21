#pragma once
// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "model.hpp"
#include "entity.hpp"
#define DEFAULT_FRONT_VECTOR glm::vec4(0.85,0.0,0.5,0.0)
#define DEFAULT_VEHICLE_ACCELERATION 10.0
#define DEFAULT_VELOCITY_VECTOR glm::vec4(0.0,0.0,0.0,0.0)
#define DEFAULT_UP_VECTOR glm::vec4(0.0f,1.0f,0.0f,0.0f)

enum CarGear{
    rest,
    forward,
    reverse
};
enum Turn{
    straight = 0,
    left = -1,
    right = 1
};
#define DEFAULT_GEAR CarGear::rest
class Vehicle: public Entity{

    public:
        Vehicle(Model object);

        Vehicle(Model object,glm::vec4 initialPosition,
            GLfloat angleX,GLfloat angleY,GLfloat angleZ,
            GLfloat sx, GLfloat sy, GLfloat sz );

        int move(float delta_time);
        void setGear(CarGear gear);
        void setTurnDirection(Turn turnDirection);
        void friction(float delta_time);


    private:
        void turn(float delta_time);

        float acceleration;
        glm::vec4 frontVector;
        glm::vec4 vehiclePosition;
        glm::vec4 resultantForce;
        glm::vec4 velocityVector;
        glm::vec4 upVector;
        CarGear gear;
        Turn turnDirection;
};