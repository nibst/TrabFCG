#include "terrain.hpp"
#include "sceneObject.hpp"
#include <glm/vec3.hpp>
#include <iostream>

Terrain::Terrain(){;}
SceneObject Terrain::generateTerrain(VAO vao){
    //AUTORIA DE TERCEIROS,FUNCAO MODIFICADA
    int count = VERTEX * VERTEX;
    std::vector<float> vertices (count*4,0.0f) ;
    std::vector<float>  normals(count*4,0.0f);
    std::vector<float> textureCoords(count*2,0.0f);
    std::vector<GLuint> indices(6*(VERTEX-1)*(VERTEX-1),0);
    int vertexPointer = 0;
    for(int i=0;i<VERTEX;i++){
        for(int j=0;j<VERTEX;j++){
            vertices[vertexPointer*4] = (float)j/((float)VERTEX - 1) * SIZE;
            vertices[vertexPointer*4+1] = 0;
            vertices[vertexPointer*4+2] = (float)i/((float)VERTEX - 1) * SIZE;
            vertices[vertexPointer*4+3] = 1;
            normals[vertexPointer*4] = 0;
            normals[vertexPointer*4+1] = 1;
            normals[vertexPointer*4+2] = 0;
            normals[vertexPointer*4+3] = 0;
            textureCoords[vertexPointer*2] = (float)j/((float)VERTEX - 1);
            textureCoords[vertexPointer*2+1] = (float)i/((float)VERTEX - 1);
            vertexPointer++;
        }
    }
    int pointer = 0;
    for(int gz=0;gz<VERTEX-1;gz++){
        for(int gx=0;gx<VERTEX-1;gx++){
            int topLeft = (gz*VERTEX)+gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz+1)*VERTEX)+gx;
            int bottomRight = bottomLeft + 1;
            indices[pointer++] = topLeft;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = topRight;
            indices[pointer++] = topRight;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = bottomRight;
        }
    }
    glm::vec3 bbox_min = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 bbox_max = glm::vec3(1.0f,1.0f,1.0f);
    vao.loadToVAO(vertices, textureCoords, normals, indices);
    std::cout<< vertices.size() << std::endl;
    for(float v: vertices)
        std::cout<<v<<std::endl;
    SceneObject object = SceneObject("terrain",0,indices.size(),GL_TRIANGLES,vao,bbox_min,bbox_max);
    return object;
}
