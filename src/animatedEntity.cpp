
#include "animatedEntity.hpp"
AnimatedEntity::AnimatedEntity(Model object):
    Entity(object),bezier_t(INITIAL_BEZIER_T),speed(DEFAULT_SPEED){
        this->bezierPoints.push_back(DEFAULT_P1);
        this->bezierPoints.push_back(DEFAULT_P2);
        this->bezierPoints.push_back(DEFAULT_P3);
        this->bezierPoints.push_back(DEFAULT_P4);
    }
void AnimatedEntity::move(const float delta_time){
    const int degree = this->bezierPoints.size() - 1;
    this->bezier_t = bezier_t + this->speed*delta_time;
    //reverse movement
    if (this->bezier_t > 1 || bezier_t < 0){
        this->speed = -this->speed;
    }
    glm::vec4 newPosition = this->bezierPoints[0];
    for (int i = 0; i<degree+1; i++){
        newPosition.x += this->bernsteinPolynomial(i,degree)*this->bezierPoints[i].x;
        newPosition.y += this->bernsteinPolynomial(i,degree)*this->bezierPoints[i].y;
        newPosition.z += this->bernsteinPolynomial(i,degree)*this->bezierPoints[i].z;
    }
    newPosition.w =  1.0f;
    this->position = newPosition;
}

long AnimatedEntity::factorial(const int n){
    long f = 1;
    for (int i=1; i<=n; ++i)
        f *= i;
    return f;
}

float AnimatedEntity::bernsteinPolynomial(const int k,const int n){
    return (this->factorial(n)/(this->factorial(k)*(this->factorial(n-k))))*pow(this->bezier_t,k)*pow(1-this->bezier_t,n-k);
}

void AnimatedEntity::setBezierPoints(const std::vector<glm::vec4> points){
    
    std::vector<glm::vec4> newBezierPoints;
    for(glm::vec4 point : points){
        newBezierPoints.push_back(point);
    }
    this->bezierPoints = newBezierPoints;
}