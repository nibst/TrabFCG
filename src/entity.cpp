
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
    this->computeAABB();
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
    this->computeAABB();
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
    this->computeAABB();
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
glm::vec3 Entity::getScales(){
    return glm::vec3(this->scaleX,this->scaleY,this->scaleZ);
}
void Entity::setPosition(glm::vec4 position){
    this->position = position;
}
void Entity::populateCorners(glm::vec4 corners[]){
    glm::vec3 bboxMin  = this->getObject().getBboxMin();
    glm::vec3 bboxMax  = this->getObject().getBboxMax();
    corners[0] = glm::vec4(bboxMin,1.0);
    corners[1] = glm::vec4(bboxMin.x, bboxMin.y, bboxMax.z,1.0);
    corners[2] = glm::vec4(bboxMin.x, bboxMax.y, bboxMin.z,1.0);
    corners[3] = glm::vec4(bboxMax.x, bboxMin.y, bboxMin.z,1.0);
    corners[4] = glm::vec4(bboxMin.x, bboxMax.y, bboxMax.z,1.0);
    corners[5] = glm::vec4(bboxMax.x, bboxMin.y, bboxMax.z,1.0);
    corners[6] = glm::vec4(bboxMax.x, bboxMax.y, bboxMin.z,1.0);
    corners[7] = glm::vec4(bboxMax,1.0);
}
void Entity::computeAABB(){
    glm::vec4 corners[8];

    populateCorners(corners);

    const float minval = std::numeric_limits<float>::min();
    const float maxval = std::numeric_limits<float>::max();

    glm::vec3 AABB_min = glm::vec3(maxval,maxval,maxval);
    glm::vec3 AABB_max = glm::vec3(minval,minval,minval);
    // Transform all of the corners, and keep track of the greatest and least
    // values we see on each coordinate axis.
    for(int i = 0; i < 8; i++) {
        glm::vec4 transformed = this->getTransformationMatrix() * corners[i];
        AABB_min.x = std::min(AABB_min.x, transformed.x);
        AABB_min.y = std::min(AABB_min.y, transformed.y);
        AABB_min.z = std::min(AABB_min.z, transformed.z);
        AABB_max.x = std::max(AABB_max.x, transformed.x);
        AABB_max.y = std::max(AABB_max.y, transformed.y);
        AABB_max.z = std::max(AABB_max.z, transformed.z);
    }
    this->AABBMax = AABB_max;
    this->AABBMin = AABB_min;
}
glm::vec3 Entity::getAABBMax(){
    return this->AABBMax;
}

glm::vec3 Entity::getAABBMin(){
    return this->AABBMin;
}