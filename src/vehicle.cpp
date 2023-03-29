
#include "vehicle.hpp"
Vehicle::Vehicle(Model object, glm::vec4 initialPosition,
                 GLfloat angleX, GLfloat angleY, GLfloat angleZ,
                 GLfloat sx, GLfloat sy, GLfloat sz) : Entity(object, initialPosition, angleX, angleY, angleZ, sx, sy, sz),
                                                       frontVector(DEFAULT_FRONT_VECTOR), acceleration(DEFAULT_VEHICLE_ACCELERATION),
                                                       gear(DEFAULT_GEAR), velocityVector(DEFAULT_VELOCITY_VECTOR), upVector(DEFAULT_UP_VECTOR),
                                                       turnDirection(Turn::straight),lateral_friction_factor(DEFAULT_LATERAL_FRICTION),max_speed(DEFAULT_MAX_SPEED),
                                                       nitroFuel(DEFAULT_NITRO_FUEL),isNitroActive(false)
{
    ;
}

Vehicle::Vehicle(Model object) : Entity(object),
                                 frontVector(DEFAULT_FRONT_VECTOR), acceleration(DEFAULT_VEHICLE_ACCELERATION),
                                 gear(DEFAULT_GEAR), velocityVector(DEFAULT_VELOCITY_VECTOR), upVector(DEFAULT_UP_VECTOR),
                                 turnDirection(Turn::straight),lateral_friction_factor(DEFAULT_LATERAL_FRICTION),max_speed(DEFAULT_MAX_SPEED),
                                 nitroFuel(DEFAULT_NITRO_FUEL),isNitroActive(false)
{
    ;
}
void Vehicle::setGear(CarGear gear)
{
    this->gear = gear;
}

void Vehicle::move(float delta_time){
    this->friction(delta_time);
    this->turn(delta_time);
    if (this->isNitroActive){
        this->nitroFuel-=delta_time*5.5;
    }
    if (this->nitroFuel<= 0)
        this->stopNitro();
    float orientation = 1;
    if (this->gear == CarGear::reverse)
        orientation = -1;
    if (this->gear == CarGear::rest)
        orientation = 0;
    // printf("orientation %f\n",orientation);

    glm::vec4 accelerationVector = this->frontVector * orientation * this->acceleration;

    float current_speed = norm(this->velocityVector);
    // printf("current_Speed %f\n",current_speed);
    if (current_speed < this->max_speed)
    {
        this->velocityVector += accelerationVector * delta_time; // v = v0 + a*t
    }
    this->position += this->velocityVector * delta_time;
}
void Vehicle::setTurnDirection(Turn turnDirection)
{
    this->turnDirection = turnDirection;
}
static glm::vec4 lerp(const glm::vec4 A, const glm::vec4 B, float t)
{
    return A * t + B * (1.f - t);
}

void Vehicle::turn(float delta_time)
{
    const float steerSensitivity = 0.04;
    float steer_angle = this->turnDirection * steerSensitivity;
    this->angleY -= steer_angle;
    glm::vec4 newFrontVector = this->frontVector * Matrix_Rotate_Y(steer_angle); // this works until there is verticality on car movements
    float amount = delta_time * 0.1;
    this->frontVector = lerp(this->frontVector, newFrontVector, amount);
}

void Vehicle::friction(float delta_time)
{
    glm::vec4 right_vector = crossproduct(this->frontVector, this->upVector);
    
    glm::vec4 lateral_velocity = right_vector * dotproduct(this->velocityVector, right_vector);
    glm::vec4 lateral_friction = -lateral_velocity * this->lateral_friction_factor;

    float backwards_friction_factor = 0.2f;
    glm::vec4 backwards_friction = -this->velocityVector * backwards_friction_factor;

    // Friction should be calculated before you apply the acceleration
    this->velocityVector += (backwards_friction + lateral_friction) * delta_time;
}
void Vehicle::setFrontVector(glm::vec4 frontVector)
{
    this->frontVector = frontVector;
}
glm::vec4 Vehicle::getFrontVector() { return this->frontVector; }
glm::vec4 Vehicle::getUpVector() { return this->upVector; }

void Vehicle::hitObject()
{
    //this->acceleration = 0.0;
    this->velocityVector -= this->velocityVector * 1.8f;
}
void Vehicle::drift(){
    this->lateral_friction_factor = DEFAULT_DRIFT_FRICTION;
}
void Vehicle::stopDrift(){
    this->lateral_friction_factor = DEFAULT_LATERAL_FRICTION;
}
void Vehicle::nitro(){
    if (this->nitroFuel>=0){
        this->acceleration = this->acceleration*3;
        this->max_speed = this->max_speed*2;
        this->isNitroActive = true;
    }


}
void Vehicle::stopNitro(){
    
    if (this->isNitroActive){
        this->acceleration = DEFAULT_VEHICLE_ACCELERATION;
        this->max_speed = DEFAULT_MAX_SPEED;
        this->isNitroActive = false;
    }
    

}
void Vehicle::resetModifications(){

    this->setTurnDirection(Turn::straight);
    this->setGear(CarGear::rest);
    this->stopNitro();
    this->stopDrift();

}
float Vehicle::getNitroFuel(){
    return this->nitroFuel;
}

float Vehicle::getSpeed(){
    return norm(this->velocityVector);
}