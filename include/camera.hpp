#pragma once
#include <glm/vec4.hpp>

class Camera{

    private:
        glm::vec4 camera_position_c;  // Ponto "c", centro da câmera
        glm::vec4 camera_lookat_l; // Ponto "l", para onde a câmera (look-at) estará sempre olhando
        glm::vec4 camera_view_vector;  // Vetor "view", sentido para onde a câmera está virada
        glm::vec4 camera_up_vector;   // Vetor "up" fixado para apontar para o "céu" (eito Y global)
};