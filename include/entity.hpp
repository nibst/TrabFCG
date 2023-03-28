
#pragma once
#include "model.hpp"
#include "matrices.h"
typedef struct serializedEntity{
    GLint modelID;
    GLfloat posx;
    GLfloat posy;
    GLfloat posz;
    GLfloat posw;
    GLfloat angleX;
    GLfloat angleY;
    GLfloat angleZ;
    GLfloat sx;
    GLfloat sy;
    GLfloat sz; 
}SerializedEntity;

class Entity{
    public:
        
        Entity(Model object);
        //sx,sy,sz = scale 
        Entity(
            Model object,glm::vec4 initialPosition,
            GLfloat angleX,GLfloat angleY,GLfloat angleZ,
            GLfloat sx, GLfloat sy, GLfloat sz 
        );
        //for deserialization
        Entity(const SerializedEntity serializedEntity, std::map<GLint,Model> possibleModels);
        void increasePosition(GLfloat x, GLfloat y, GLfloat z);
        void rotate(GLfloat angleX,GLfloat angleY,GLfloat angleZ);
        void scale(GLfloat sx,GLfloat sy, GLfloat sz);
        Model getObject();
        void setObject(Model object);
        glm::mat4 getTransformationMatrix();
        glm::vec4 getPosition();
        GLfloat getAngleX();
        GLfloat getAngleY();
        GLfloat getAngleZ();

        SerializedEntity* serialize();
    protected:
        
        Model object;
        glm::vec4 position;
        GLfloat angleX;
        GLfloat angleY;
        GLfloat angleZ;
        GLfloat scaleX;
        GLfloat scaleY;
        GLfloat scaleZ;


};