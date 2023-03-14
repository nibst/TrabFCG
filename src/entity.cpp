
#include "entity.hpp"
Entity::Entity(SceneObject object){
    this->object = object;
    this->position = glm::vec4(5.0f,-1.0f,5.0f,1.0f);
    this->angleX = 0.0f;
    this->angleY = 1.0f;
    this->angleZ = 0.0f;
    this->scaleX = 1.0f;
    this->scaleY = 1.0f;
    this->scaleZ = 1.0f;
}
Entity::Entity(
            SceneObject object,glm::vec4 initialPosition,
            GLfloat angleX,GLfloat angleY,GLfloat angleZ,
            float sx, float sy, float sz 
        )
{
    this->object = object;
    this->position = initialPosition;
    this->angleX = angleX;
    this->angleY = angleY;
    this->angleZ = angleZ;
    this->scaleX = sx;
    this->scaleY = sy;
    this->scaleZ = sz;
}

void Entity::increasePosition(GLfloat x, GLfloat y, GLfloat z){
    this->position.x += x;
    this->position.y += y;
    this->position.z += z;
}
void Entity::rotate(GLfloat angleX,GLfloat angleY,GLfloat angleZ){
    this->angleX += angleX;
    this->angleY += angleY;
    this->angleZ += angleZ;
}
void Entity::scale(GLfloat sx,GLfloat sy, GLfloat sz){
    this->scaleX += sx;
    this->scaleY += sy;
    this->scaleZ += sz;
}
SceneObject Entity::getObject(){
    return this->object;
}
glm::mat4 Entity::getTransformationMatrix(){
    glm::mat4 matrix = Matrix_Identity();
    matrix = matrix*Matrix_Translate(position.x,position.y,position.z);
    matrix = matrix*Matrix_Rotate_X(angleX);
    matrix = matrix*Matrix_Rotate_Y(angleY);
    matrix = matrix*Matrix_Rotate_Z(angleZ);
    matrix = matrix*Matrix_Scale(scaleX,scaleY,scaleZ);
	return matrix;

		
}