#pragma once
#include "sceneObject.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

class Renderer{
    public:
        Renderer();
        void render(SceneObject object,glm::mat4 model);
        GLuint g_GpuProgramID = 0;
        GLint g_model_uniform;
        GLint g_view_uniform;
        GLint g_projection_uniform;
        GLint g_object_id_uniform;
        GLint g_bbox_min_uniform;
        GLint g_bbox_max_uniform;
    
    private:
        GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
        void LoadShadersFromFiles();
        GLuint LoadShader_Vertex(const char* filename);
        GLuint LoadShader_Fragment(const char* filename);
        void LoadShader(const char* filename, GLuint shader_id);
        // Variáveis que definem um programa de GPU (shaders). Veja função LoadShadersFromFiles().


};