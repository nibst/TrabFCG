#include "lookAtCamera.hpp"
LookAtCamera::LookAtCamera(Entity *object)
:Camera(),lookAtObject(object),cameraDistance(DEFAULT_CAMERA_DISTANCE)
{;}

void LookAtCamera::setLookAt(glm::vec4 position){
    this->lookAtObject->getPosition() = position;
}

void LookAtCamera::move(){

    this->calculateCameraPosition();
    this->theta = M_PI- (this->lookAtObject->getAngleY() + this->theta);
    this->view_vector = this->lookAtObject->getPosition() - this->position_c; // Vetor "view", sentido para onde a câmera está

}

void LookAtCamera::calculateCameraPosition(){
    float verticalDistance = cameraDistance*cos(this->phi);
    float horizontalDistance =cameraDistance*sin(this->phi);
    float angle = this->lookAtObject->getAngleY() + this->theta;
    float offsetX = horizontalDistance * sin(angle);
    float offsetZ = horizontalDistance * cos(angle);
    this->position_c.x = this->lookAtObject->getPosition().x - offsetX+0.01;
    this->position_c.z = this->lookAtObject->getPosition().z - offsetZ +0.01;
    this->position_c.y = this->lookAtObject->getPosition().y + verticalDistance+0.01;

}


void LookAtCamera::calculateZoom(double yoffset){
    // Atualizamos a distância da câmera para a origem utilizando a
    // movimentação da "rodinha", simulando um ZOOM.
    this->cameraDistance -= 0.1f*yoffset;

    // Uma câmera look-at nunca pode estar exatamente "em cima" do ponto para
    // onde ela está olhando, pois isto gera problemas de divisão por zero na
    // definição do sistema de coordenadas da câmera. Isto é, a variável abaixo
    // nunca pode ser zero. Versões anteriores deste código possuíam este bug,
    // o qual foi detectado pelo aluno Vinicius Fraga (2017/2).
    const float verysmallnumber = std::numeric_limits<float>::epsilon();
    if ( this->cameraDistance < verysmallnumber)
         this->cameraDistance = verysmallnumber;
}