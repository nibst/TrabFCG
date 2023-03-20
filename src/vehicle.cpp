
#include "vehicle.hpp"
Vehicle::Vehicle(Model object,glm::vec4 initialPosition,
            GLfloat angleX,GLfloat angleY,GLfloat angleZ,
            GLfloat sx, GLfloat sy, GLfloat sz ): 
            Entity(object,initialPosition,angleX,angleY,angleZ,sx,sy,sz),
            frontVector(DEFAULT_FRONT_VECTOR),acceleration(DEFAULT_VEHICLE_ACCELERATION),
            gear(DEFAULT_GEAR),velocityVector(DEFAULT_VELOCITY_VECTOR)
{;}

Vehicle::Vehicle(Model object): 
            Entity(object),
            frontVector(DEFAULT_FRONT_VECTOR),acceleration(DEFAULT_VEHICLE_ACCELERATION),
            gear(DEFAULT_GEAR),velocityVector(DEFAULT_VELOCITY_VECTOR)
{;}
void Vehicle::setGear(CarGear gear){
    this->gear = gear;
}

int Vehicle::move(float delta_time){
    float max_speed = 30;
    float orientation = 1;
    if (this->gear == CarGear::reverse)
        orientation = -1;
    if (this->gear == CarGear::rest)
        orientation = 0;
    printf("orientation %f\n",orientation);

    this->resultantForce = this->frontVector * orientation * this->acceleration;
    printf("resultantForce %f,%f,%f,%f\n",resultantForce.x,resultantForce.y,resultantForce.z,resultantForce.w);

    float current_speed = norm(this->velocityVector);
    printf("current_Speed %f\n",current_speed);
    if (current_speed < max_speed){
        this->velocityVector += this->resultantForce * delta_time;
    }

    this->position += this->velocityVector * delta_time;
    
} 
