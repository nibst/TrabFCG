
#include "entity.hpp"
Entity::Entity(Model object){
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
            Model object,glm::vec4 initialPosition,
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
Entity::Entity(const SerializedEntity serializedEntity, std::map<GLint,Model> possibleModels){
    //deserialize and load
    this->object = possibleModels[serializedEntity.modelID];
    this->object.setObjectID(serializedEntity.modelID); // for same models with different texture

    this->position.x = serializedEntity.posx;
    this->position.y = serializedEntity.posy;
    this->position.z = serializedEntity.posz;
    this->position.w = serializedEntity.posw;

    this->angleX = serializedEntity.angleX;
    this->angleY = serializedEntity.angleY;
    this->angleZ =serializedEntity.angleZ;

    this->scaleX = serializedEntity.sx;
    this->scaleY = serializedEntity.sy;
    this->scaleZ = serializedEntity.sz;
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
Model Entity::getObject(){
    return this->object;
}
void Entity::setObject(Model object){
    this->object = object;
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
glm::vec4 Entity::getPosition(){
    return this->position;
}

GLfloat Entity::getAngleX(){
    return this->angleX;
}

GLfloat Entity::getAngleY(){
    return this->angleY;
}

GLfloat Entity::getAngleZ(){
    return this->angleZ;
}
void rotateAroundAxis();


SerializedEntity* Entity::serialize(){
    SerializedEntity* serializedEntity = (SerializedEntity*) malloc(sizeof(SerializedEntity));
    serializedEntity->modelID = this->object.getID();

    serializedEntity->posx = this->position.x;
    serializedEntity->posy = this->position.y;
    serializedEntity->posz = this->position.z;
    serializedEntity->posw = this->position.w;

    serializedEntity->angleX = this->angleX;
    serializedEntity->angleY = this->angleY;
    serializedEntity->angleZ = this->angleZ;

    serializedEntity->sx = this->scaleX;
    serializedEntity->sy = this->scaleY;
    serializedEntity->sz = this->scaleZ;
    return serializedEntity;
}
