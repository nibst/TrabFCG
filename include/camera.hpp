#pragma once
#include "matrices.h"
#define DEFAULT_POS glm::vec4(2.0f,3.0f,2.0f,1.0f)
#define DEFAULT_VIEW glm::vec4(0.0f,0.0f,3.5f,0.0f)
#define DEFAULT_PHI 1.0f
#define DEFAULT_THETA 0.0f
#define DEFAULT_NEAR -0.1f
#define DEFAULT_FAR -150.f
#define DEFAULT_RATIO 1.0f
#define DEFAULT_UP glm::vec4(0.0f,1.0f,0.0f,0.0f)
#define DEFAULT_SPEED 14.5f
#define DEFAULT_FOV 3.141592/3.0f

class Camera{
    public:
        Camera();
        Camera(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector);
        glm::mat4 getViewMatrix();
        glm::mat4 getPerspectiveMatrix();
        void rotate(float dphi,float dtheta);
        void setCenterPosition(glm::vec4 position);
        glm::vec4 getCenterPosition();
        void setScreenRatio(float screenRatio);
        glm::vec4 getViewVector();
        virtual void move();
        virtual void calculateZoom(double yoffset);
        float speed;

    protected:
        float phi;
        float theta;
        float nearplane;  // Posição do "near plane"
        float farplane; // Posição do "far plane"
        float field_of_view;
        float screenRatio;
        glm::vec4 position_c ;  // Ponto "c", centro da câmera
        glm::vec4 view_vector;  // Vetor "view", sentido para onde a câmera está virada
        glm::vec4 up_vector;   // Vetor "up" fixado para apontar para o "céu" (eito Y global)
};