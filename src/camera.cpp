#include "camera.hpp"
Camera::Camera():
    position_c(DEFAULT_POS),view_vector(DEFAULT_VIEW),up_vector(DEFAULT_UP),
    phi(DEFAULT_PHI),theta(DEFAULT_THETA),nearplane(DEFAULT_NEAR),farplane(DEFAULT_FAR),
    field_of_view(DEFAULT_FOV),screenRatio(DEFAULT_RATIO),speed(DEFAULT_SPEED){;}

Camera::Camera(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector):
    position_c(position_c),view_vector(view_vector),up_vector(up_vector),
    phi(DEFAULT_PHI),theta(DEFAULT_THETA),nearplane(DEFAULT_NEAR),farplane(DEFAULT_FAR),
    field_of_view(DEFAULT_FOV),screenRatio(DEFAULT_RATIO),speed(DEFAULT_SPEED){;}

glm::mat4 Camera::getViewMatrix(){
    return Matrix_Camera_View(this->position_c,this->view_vector,this->up_vector);
}
glm::mat4 Camera::getPerspectiveMatrix(){
    return Matrix_Perspective(this->field_of_view,this->screenRatio,this->nearplane,this->farplane);
}
void Camera::setCenterPosition(glm::vec4 position){
    this->position_c = position;
}
glm::vec4 Camera::getCenterPosition(){
    return this->position_c;
}
void Camera::rotate(float dphi,float dtheta){
    this->phi = dphi;
    this->theta = dtheta;
}


void Camera::setScreenRatio(float screenRatio){
    this->screenRatio = screenRatio;
}

glm::vec4 Camera::getViewVector(){
    return this->view_vector;
}

void Camera::move(){
    ;
}
void Camera::calculateZoom(double yoffset){;}
