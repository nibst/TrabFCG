#include "camera.hpp"
#include "entity.hpp"
#define DEFAULT_CAMERA_DISTANCE 10.0f
class LookAtCamera: public Camera{
    public:
        LookAtCamera(Entity *object);
        void setLookAt(glm::vec4 position);
        void setCameraDistance(float distance);
        float getCameraDistance();
        void calculateCameraPosition();
        virtual void rotate(float dphi,float dtheta);
        virtual void calculateZoom(double yoffset);


        void move();

    private:
        float cameraDistance; //for look_at camera
        Entity *lookAtObject; //entity to look at
};