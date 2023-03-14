
#pragma once
#include "sceneObject.hpp"
#include "matrices.h"
class Entity{
    public:
        Entity(SceneObject object);
        //sx,sy,sz = scale 
        Entity(
            SceneObject object,glm::vec4 initialPosition,
            GLfloat angleX,GLfloat angleY,GLfloat angleZ,
            GLfloat sx, GLfloat sy, GLfloat sz 
        );
        void increasePosition(GLfloat x, GLfloat y, GLfloat z);
        void rotate(GLfloat angleX,GLfloat angleY,GLfloat angleZ);
        void scale(GLfloat sx,GLfloat sy, GLfloat sz);
        SceneObject getObject();
        glm::mat4 getTransformationMatrix();
    protected:
        SceneObject object;
        glm::vec4 position;
        GLfloat angleX;
        GLfloat angleY;
        GLfloat angleZ;
        GLfloat scaleX;
        GLfloat scaleY;
        GLfloat scaleZ;


};