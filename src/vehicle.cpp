
#include "vehicle.hpp"
Vehicle::Vehicle(SceneObject object,glm::vec4 initialPosition,
            GLfloat angleX,GLfloat angleY,GLfloat angleZ,
            GLfloat sx, GLfloat sy, GLfloat sz ): Entity(object,initialPosition,angleX,angleY,angleZ,sx,sy,sz)
{}

int Vehicle::move(){;} 