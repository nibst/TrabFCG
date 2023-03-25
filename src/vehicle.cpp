
#include "vehicle.hpp"
Vehicle::Vehicle(Model object,glm::vec4 initialPosition,
            GLfloat angleX,GLfloat angleY,GLfloat angleZ,
            GLfloat sx, GLfloat sy, GLfloat sz ): 
            Entity(object,initialPosition,angleX,angleY,angleZ,sx,sy,sz),
            frontVector(DEFAULT_FRONT_VECTOR),acceleration(DEFAULT_VEHICLE_ACCELERATION),
            gear(DEFAULT_GEAR),velocityVector(DEFAULT_VELOCITY_VECTOR),upVector(DEFAULT_UP_VECTOR),
            turnDirection(Turn::straight)
{;}

Vehicle::Vehicle(Model object): 
            Entity(object),
            frontVector(DEFAULT_FRONT_VECTOR),acceleration(DEFAULT_VEHICLE_ACCELERATION),
            gear(DEFAULT_GEAR),velocityVector(DEFAULT_VELOCITY_VECTOR),upVector(DEFAULT_UP_VECTOR),
            turnDirection(Turn::straight)
{;}
void Vehicle::setGear(CarGear gear){
    this->gear = gear;
}

int Vehicle::move(float delta_time){
    this->friction(delta_time);
    this->turn(delta_time);
    float max_speed = 100;
    float orientation = 1;
    if (this->gear == CarGear::reverse)
        orientation = -1;
    if (this->gear == CarGear::rest)
        orientation = 0;
    //printf("orientation %f\n",orientation);

    glm::vec4 accelerationVector = this->frontVector * orientation * this->acceleration;

    float current_speed = norm(this->velocityVector);
    //printf("current_Speed %f\n",current_speed);
    if (current_speed < max_speed){
        this->velocityVector += accelerationVector * delta_time; // v = v0 + a*t
    }
    this->position += this->velocityVector * delta_time;

} 
void Vehicle::setTurnDirection(Turn turnDirection){
    this->turnDirection = turnDirection;
}
static glm::vec4 lerp(const glm::vec4 A, const glm::vec4 B, float t ){
    return A*t + B*(1.f-t) ;
}

void Vehicle::turn(float delta_time){
    float steer_angle = this->turnDirection * 0.03;
    this->angleY-= steer_angle;
    glm::vec4 newFrontVector = this->frontVector * Matrix_Rotate_Y(steer_angle);//this works until there is verticality on car movements
    float amount = delta_time * 0.1;
    this->frontVector = lerp(this->frontVector, newFrontVector, amount);
}

void Vehicle::friction(float delta_time){
    glm::vec4 right_vector = crossproduct(this->frontVector,this->upVector);
    float lateral_friction_factor = 1.5f;
    glm::vec4 lateral_velocity = right_vector * dotproduct(this->velocityVector, right_vector);
    glm::vec4 lateral_friction = -lateral_velocity * lateral_friction_factor;

    float backwards_friction_factor = 0.2f;
    glm::vec4 backwards_friction = -this->velocityVector * backwards_friction_factor;

    // Friction should be calculated before you apply the acceleration
    this->velocityVector += (backwards_friction + lateral_friction) * delta_time;

}
 
