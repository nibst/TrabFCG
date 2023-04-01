#include "freeCamera.hpp"

FreeCamera::FreeCamera(glm::vec4 position):Camera(){
    this->position_c = position;
}
void FreeCamera::calculateZoom(double yoffset){;}
void FreeCamera::move(){
    float x = cos(this->phi)*sin(this->theta);
    float y = sin(this->phi);
    float z = cos(this->phi)*cos(this->theta);
    this->view_vector = glm::vec4(x,-y,z,0.0f); // Vetor "view", sentido para onde a câmera está virada
}