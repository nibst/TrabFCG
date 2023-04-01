#include "camera.hpp"

class FreeCamera : public Camera{
    public:

        FreeCamera(glm::vec4 position);    
        virtual void calculateZoom(double yoffset);

        void move();
    private:

};