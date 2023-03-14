#pragma once
#include <glad/glad.h>
#include <vector>

class VAO{
    public:
        VAO();
        void loadToVAO(std::vector<float>  model_coefficients,std::vector<float>  texture_coefficients,std::vector<float> normal_coefficients,std::vector<GLuint> indices);
        void bindAttrib(GLuint location, GLint dimensions,std::vector<float> attribData);
        void bindIndices(std::vector<GLuint> indices);
        void bind();
        void unbind();
        //void deleteVAO();
        GLuint getID();
        
    private:
        GLuint id;    
};