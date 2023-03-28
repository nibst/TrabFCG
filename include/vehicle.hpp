#pragma once
// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "model.hpp"
#include "entity.hpp"
#define DEFAULT_FRONT_VECTOR glm::vec4(1.58,0.0,1.0,0.0)
#define DEFAULT_VEHICLE_ACCELERATION 10.0
#define DEFAULT_VELOCITY_VECTOR glm::vec4(0.0,0.0,0.0,0.0)
#define DEFAULT_UP_VECTOR glm::vec4(0.0f,1.0f,0.0f,0.0f)
#define DEFAULT_LATERAL_FRICTION 2.5f
#define DEFAULT_DRIFT_FRICTION 0.5f
#define DEFAULT_MAX_SPEED 100.0f
#define DEFAULT_NITRO_FUEL 100.0f

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

        void move(float delta_time);
        void setGear(CarGear gear);
        void setTurnDirection(Turn turnDirection);
        void hitObject();
        void setFrontVector(glm::vec4 frontVector);
        glm::vec4 getFrontVector();
        glm::vec4 getUpVector();
        void drift();
        void nitro();
        float getNitroFuel();
        void resetModifications();
        float getSpeed();



    private:
        void stopDrift();
        void stopNitro();
        void friction(float delta_time);
        void turn(float delta_time);
        float lateral_friction_factor;
        float max_speed;
        float acceleration;
        float nitroFuel;
        bool isNitroActive;
        glm::vec4 frontVector;
        glm::vec4 vehiclePosition;
        glm::vec4 velocityVector;
        glm::vec4 upVector;
        CarGear gear;
        Turn turnDirection;
};