
#include "vehicle.hpp"
Vehicle::Vehicle(Model object,glm::vec4 initialPosition,
            GLfloat angleX,GLfloat angleY,GLfloat angleZ,
            GLfloat sx, GLfloat sy, GLfloat sz ): 
            Entity(object,initialPosition,angleX,angleY,angleZ,sx,sy,sz),
            frontVector(DEFAULT_FRONT_VECTOR),acceleration(DEFAULT_VEHICLE_ACCELERATION)
{;}

Vehicle::Vehicle(Model object): 
            Entity(object),
            frontVector(DEFAULT_FRONT_VECTOR),acceleration(DEFAULT_VEHICLE_ACCELERATION)
{;}

int Vehicle::move(CarGear gear){
    this->frontVector;
    float orientation = 1;
    if (gear == CarGear::normal)
        orientation = -1;
    glm::vec4 acceleration_vector = this->frontVector * orientation * this->acceleration;
    
} 