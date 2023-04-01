//     Universidade Federal do Rio Grande do Sul
//             Instituto de Informática
//       Departamento de Informática Aplicada
//
//    INF01047 Fundamentos de Computação Gráfica
//               Prof. Eduardo Gastal
//
//                   LABORATÓRIO 5
//

// Arquivos "headers" padrões de C podem ser incluídos em um
// programa C++, sendo necessário somente adicionar o caractere
// "c" antes de seu nome, e remover o sufixo ".h". Exemplo:
//    #include <stdio.h> // Em C
//  vira
//    #include <cstdio> // Em C++
//
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Headers abaixo são específicos de C++
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>  // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h> // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

#include <stb_image.h>

// Headers locais, definidos na pasta "include/"
#include "utils.h"
#include "matrices.h"
#include "windowManager.hpp"
#include "model.hpp"
#include "renderer.hpp"
#include "terrain.hpp"
#include "entity.hpp"
#include "camera.hpp"
#include "vehicle.hpp"
#include "lookAtCamera.hpp"
#include "animatedEntity.hpp"
#include "collisions.hpp"
#include "freeCamera.hpp"

// Declaração de várias funções utilizadas em main().  Essas estão definidas
// logo após a definição de main() neste arquivo.
void LoadTextureImage(const char *filename);                                 // Função que carrega imagens de textura

GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); // Cria um programa de GPU
void PrintObjModelInfo(ObjModel *);                                          // Função para debugging

// Declaração de funções auxiliares para renderizar texto dentro da janela
// OpenGL. Estas funções estão definidas no arquivo "textrendering.cpp".
void TextRendering_Init();
float TextRendering_LineHeight(GLFWwindow *window);
float TextRendering_CharWidth(GLFWwindow *window);
void TextRendering_PrintString(GLFWwindow *window, const std::string &str, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrix(GLFWwindow *window, glm::mat4 M, float x, float y, float scale = 1.0f);
void TextRendering_PrintVector(GLFWwindow *window, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProduct(GLFWwindow *window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProductMoreDigits(GLFWwindow *window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProductDivW(GLFWwindow *window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);

// Funções abaixo renderizam como texto na janela OpenGL algumas matrizes e
// outras informações do programa. Definidas após main().
void TextRendering_ShowModelViewProjection(GLFWwindow *window, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 p_model);
void TextRendering_ShowEulerAngles(GLFWwindow *window);
void TextRendering_ShowProjection(GLFWwindow *window);
void TextRendering_ShowFramesPerSecond(GLFWwindow *window);

// Funções callback para comunicação com o sistema operacional e interação do
// usuário. Veja mais comentários nas definições das mesmas, abaixo.
void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
void ErrorCallback(int error, const char *description);
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

void writeEntities(std::ostream &os, const std::vector<SerializedEntity> &vec);
std::vector<SerializedEntity> readEntities(std::istream &inputStream);

void writeEntities(std::ostream &os, const std::vector<SerializedEntity> &vec);
std::vector<SerializedEntity> readEntities(std::istream &inputStream);
void showCollisionBoxes(std::vector<Entity> walls, Entity kart, GLuint kartVAO, GLuint wallVAO, GLuint g_GpuProgramID);

GLuint buildVisualizationOfBbox(Model model);
// Abaixo definimos variáveis globais utilizadas em várias funções do código.


// Ângulos de Euler que controlam a rotação de um dos cubos da cena virtual
float g_AngleX = 0.0f;
float g_AngleY = 0.0f;
float g_AngleZ = 0.0f;

// "g_LeftMouseButtonPressed = true" se o usuário está com o botão esquerdo do mouse
// pressionado no momento atual. Veja função MouseButtonCallback().
bool g_LeftMouseButtonPressed = false;
bool g_RightMouseButtonPressed = false;  // Análogo para botão direito do mouse

// Variáveis que definem a câmera em coordenadas esféricas, controladas pelo
// usuário através do mouse (veja função CursorPosCallback()). A posição
// efetiva da câmera é calculada dentro da função main(), dentro do loop de
// renderização.
float g_CameraTheta = 0.0f;    // Ângulo no plano ZX em relação ao eixo Z
float g_CameraPhi = 1.15f;     // Ângulo em relação ao eixo Y'
float g_CameraDistance = 3.5f; // Distância da câmera para a origem


// Variável que controla o tipo de projeção utilizada: perspectiva ou ortográfica.
bool g_UsePerspectiveProjection = true;
bool g_UseFreeCamera = false;
// Variável que controla se o texto informativo será mostrado na tela.
bool g_ShowInfoText = true;

bool tecla_A_pressionada = false, tecla_D_pressionada = false, tecla_S_pressionada = false,
     tecla_W_pressionada = false, tecla_space_pressionada = false, tecla_J_pressionada = false, tecla_ctrl_pressionada = false;
// Número de texturas carregadas pela função LoadTextureImage()
GLuint g_NumLoadedTextures = 0;
Camera *camera = new Camera();
int main(int argc, char *argv[])
{

    WindowManager *windowManager = new WindowManager();
    // Definimos o callback para impressão de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    GLFWwindow *window = windowManager->createWindow();

    // Definimos a função de callback que será chamada sempre que o usuário
    // pressionar alguma tecla do teclado ...
    glfwSetKeyCallback(window, KeyCallback);
    // ... ou clicar os botões do mouse ...
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    // ... ou movimentar o cursor do mouse em cima da janela ...
    glfwSetCursorPosCallback(window, CursorPosCallback);
    // ... ou rolar a "rodinha" do mouse.
    glfwSetScrollCallback(window, ScrollCallback);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, 800, 600); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    //
    Renderer renderer = Renderer();

    // Carregamos duas imagens para serem utilizadas como textura
    LoadTextureImage("../../data/grass.jpeg");                       // TextureImage0
    LoadTextureImage("../../data/textures/asphalt_road.png");        // TextureImage1
    LoadTextureImage("../../data/textures/marioKartMap.jpg");        // TextureImage2
    LoadTextureImage("../../data/textures/GoKart_[Albedo].tga.png"); // TextureImage3
    LoadTextureImage("../../data/textures/anotherbrick.jpg");        // TextureImage4
    LoadTextureImage("../../data/textures/stripes.jpg");             // TextureImage5
    LoadTextureImage("../../data/textures/rainbowBricks.jpg");       // TextureImage6
    LoadTextureImage("../../data/textures/cow_texture.jpg");         // TextureImage7
    LoadTextureImage("../../data/textures/Textures.png");            // TextureImage8
#define SPHERE 0
#define KART 1
#define PLANE 2
#define OUTERWALL 3
#define INNERWALL 4
#define COW 5
#define FINISH 6

    std::map<GLint, Model> models;
    // Construímos a representação de objetos geométricos através de malhas de triângulos
    Terrain terrain = Terrain();
    VAO terrainVao = VAO();

    Model terrainmodel;
    terrainmodel = terrain.generateTerrain(terrainVao);
    terrainmodel.setObjectID(PLANE);
    models[PLANE] = terrainmodel;

    Model kartmodel;
    kartmodel.loadFromOBJFileName("../../data/GoKart.obj"); // loadFromOBJFileName("../../data/GoKart.obj");
    kartmodel.setObjectID(KART);
    models[KART] = kartmodel;

    Vehicle kart = Vehicle(kartmodel);
    Camera *lookAtCamera = (LookAtCamera *)new LookAtCamera(&kart);
    Camera *freeCamera = (FreeCamera *)new FreeCamera(kart.getPosition());
    camera = lookAtCamera;
    // glm::vec4 front = glm::vec4(-5.32, 0.0, 8.46, 0.0);
    // front = front * 0.2f;
    // kart.setFrontVector(front);

    Model wallmodel;
    wallmodel.loadFromOBJFileName("../../data/wall.obj");
    wallmodel.setObjectID(OUTERWALL);
    models[OUTERWALL] = wallmodel;
    models[INNERWALL] = wallmodel; // usado na hora do load do arquivo binario com as paredes

    Model cowmodel;
    cowmodel.loadFromOBJFileName("../../data/cow.obj");
    cowmodel.setObjectID(COW);
    models[COW] = cowmodel;

    Model finishmodel;
    finishmodel.loadFromOBJFileName("../../data/uploads_files_3661170_gate.obj");
    finishmodel.setObjectID(FINISH);
    models[FINISH] = finishmodel;
    Entity finishLine = Entity(models[FINISH], glm::vec4(24.588949, -1.000000, 50.209194, 1.0), 0.0, 0.0, 0.0, 2.0, 1.0, 2.0);

    AnimatedEntity movingCow = AnimatedEntity(cowmodel);
    movingCow.scale(2.0, 2.0, 2.0);
    // get all walls for rendering
    std::ifstream inWalls("../../wallMap.bin", std::ios::in | std::ios::binary);
    std::vector<SerializedEntity> allSerializedWalls = readEntities(inWalls);
    inWalls.close();
    // get all obstacles for rendering
    std::ifstream inObstacles("../../obstaclesMap.bin", std::ios::in | std::ios::binary);
    std::vector<SerializedEntity> allSerializedCows = readEntities(inObstacles);
    inObstacles.close();

    std::vector<Entity> walls;
    for (SerializedEntity serialized : allSerializedWalls)
    {
        Entity entity = Entity(serialized, models);
        // printf("%d\n",entity.getObject().getID());
        walls.push_back(entity);
    }
    std::vector<Entity> obstacles;
    for (SerializedEntity serialized : allSerializedCows)
    {
        Entity entity = Entity(serialized, models);
        // printf("%d\n",entity.getObject().getID());
        obstacles.push_back(entity);
    }

    // Inicializamos o código para renderização de texto.
    TextRendering_Init();
    float prev_time = (float)glfwGetTime();
    float start = (float)glfwGetTime();
    float end;
    GLuint wallVAO = buildVisualizationOfBbox(models[OUTERWALL]);
    GLuint kartVAO = buildVisualizationOfBbox(models[KART]);
    bool gameWon = false;
    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!(windowManager->shouldCloseWindow()))
    {

        // Aqui executamos as operações de renderização

        // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
        // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto é:
        // Vermelho, Verde, Azul, Alpha (valor de transparência).
        // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
        //
        //           R     G     B     A
        glClearColor(0.2f, 0.70f, 0.8f, 1.0f);

        // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
        // e também resetamos todos os pixels do Z-buffer (depth buffer).
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
        // vértices apontados pelo VAO criado pela função BuildTrianglesAndAddToVirtualScene(). Veja
        // comentários detalhados dentro da definição de BuildTrianglesAndAddToVirtualScene().
        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de vértice e fragmentos).
        glUseProgram(renderer.g_GpuProgramID);

        // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
        // Veja slides 195-227 e 229-234 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
        float current_time = (float)glfwGetTime();
        float delta_t = current_time - prev_time;
        prev_time = current_time;
        // Realiza movimentação de objetos
        if (tecla_D_pressionada){
            kart.setTurnDirection(Turn::right);
        }
        if (tecla_S_pressionada){
            kart.setGear(CarGear::reverse);
        }
        if (tecla_A_pressionada){
            kart.setTurnDirection(Turn::left);
        }
        if (tecla_W_pressionada){
            kart.setGear(CarGear::forward);
        }
        if (tecla_ctrl_pressionada){
            kart.drift();
        }
        if (tecla_space_pressionada){
            kart.nitro();
        }
        glm::vec4 previousPos = kart.getPosition();
        kart.move(delta_t);
        //Colisoes com parede
        for (Entity wall : walls)
        {
            if (Collisions::sphereBoxCollisionTest(kart.getPosition(), 1.0f, wall))
            {
                kart.setPosition(previousPos);
                kart.hitObject();
            }
        }
        //colisao com a vaca que se move
        if (Collisions::spheresCollisionTest(movingCow.getPosition(), 1.2f, kart.getPosition(), 1.0f)){
            kart.setPosition(previousPos);
            kart.hitObject();

        }
        //colisao com todas outras vacas
        for (Entity cow : obstacles)
        {
            if (Collisions::sphereBoxCollisionTest(kart.getPosition(), 1.0f, cow))
            {
                kart.setPosition(previousPos);
                kart.hitObject();
            }
        }
        //colisao com linha de chegada
        if (Collisions::spheresCollisionTest(kart.getPosition(), 1.0f, finishLine.getPosition(),1.5))
        {
            end = (float)glfwGetTime();
            gameWon = true;
            // kart.setPosition(glm::vec4(100.0f,0.0f,0.0f,1.0f));
        }

        float lineheight = TextRendering_LineHeight(window);
        float charwidth = TextRendering_CharWidth(window);
        if (g_UseFreeCamera){

            camera  = freeCamera;
            camera->setCenterPosition(kart.getPosition()+glm::vec4(0.0,1.0,0.0,0.0));
        }
        else{
            camera  = lookAtCamera;
        }
        camera->rotate(g_CameraPhi, g_CameraTheta);
        camera->move();
        kart.resetModifications();
        movingCow.move(delta_t);

        if (gameWon)
        {
            TextRendering_PrintString(window, "TIME:  ", 0.1f - (18 + 1) * charwidth, 0.5f - lineheight, 4.0f);
            float totalTime = end - start;
            TextRendering_PrintString(window, std::to_string(totalTime), 0.4f - (10 + 1) * charwidth, 0.5f - lineheight, 4.0f);
            if (tecla_J_pressionada)
            {
                start = (float)glfwGetTime();
                kart = Vehicle(models[KART]);
                gameWon = false;
            }
        }

        // Computamos a matriz "View" utilizando os parâmetros da câmera para
        // definir o sistema de coordenadas da câmera.  Veja slides 2-14, 184-190 e 236-242 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
        glm::mat4 view = camera->getViewMatrix();

        // Agora computamos a matriz de Projeção.
        glm::mat4 projection;

        projection = camera->getPerspectiveMatrix();

        glm::mat4 model = Matrix_Identity(); // Transformação identidade de modelagem

        // Enviamos as matrizes "view" e "projection" para a placa de vídeo
        // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas são
        // efetivamente aplicadas em todos os pontos.
        glUniformMatrix4fv(renderer.g_view_uniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(renderer.g_projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));

        // Desenhamos o modelo do coelho;
        if (!gameWon)
        {
            renderer.render(kart.getObject(), kart.getTransformationMatrix());
            renderer.render(movingCow.getObject(), movingCow.getTransformationMatrix());
            renderer.render(finishLine.getObject(), finishLine.getTransformationMatrix());
            // Enviamos a matriz "model" para a placa de vídeo (GPU). Veja o
            // arquivo "shader_vertex.glsl", onde esta é efetivamente
            // aplicada em todos os pontos.

            for (Entity wall : walls)
            {
                renderer.render(wall.getObject(), wall.getTransformationMatrix());
            }
            for (Entity obstacle : obstacles)
            {
                renderer.render(obstacle.getObject(), obstacle.getTransformationMatrix());
            }
            model = Matrix_Translate(0.0f, -1.1f, 0.0f);
            renderer.render(terrainmodel, model);
            TextRendering_PrintString(window, "speed: ", 0.1f - (18 + 1) * charwidth, 1.0f - lineheight, 2.0f);
            TextRendering_PrintString(window, std::to_string(kart.getSpeed()), 0.2f - (10 + 1) * charwidth, 1.0f - lineheight, 2.0f);
            TextRendering_PrintString(window, "nitro: ", 0.8f - (18 + 1) * charwidth, 1.0f - lineheight, 2.0f);
            TextRendering_PrintString(window, std::to_string(kart.getNitroFuel()), 0.9f - (10 + 1) * charwidth, 1.0f - lineheight, 2.0f);
        }


        TextRendering_ShowFramesPerSecond(window);

        windowManager->updateWindow();
    }
    // for making map things
    /*    std::vector<SerializedEntity> allSerializedObstacles;
    for(Entity obstacle: obstacles){
        SerializedEntity* serializedObstacles = obstacle.serialize();
        allSerializedObstacles.push_back(*serializedObstacles);
    }
    std::ofstream out("../../obstaclesMap.bin", std::ios::out | std::ios::binary);
    writeEntities(out,allSerializedObstacles);
    out.close();
    */

    free(camera);
    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // Fim do programa
    return 0;
}

// Função que carrega uma imagem para ser utilizada como textura
void LoadTextureImage(const char *filename)
{
    printf("Carregando imagem \"%s\"... ", filename);

    // Primeiro fazemos a leitura da imagem do disco
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);

    if (data == NULL)
    {
        fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }

    printf("OK (%dx%d).\n", width, height);

    // Agora criamos objetos na GPU com OpenGL para armazenar a textura
    GLuint texture_id;
    GLuint sampler_id;
    glGenTextures(1, &texture_id);
    glGenSamplers(1, &sampler_id);

    // Veja slides 95-96 do documento Aula_20_Mapeamento_de_Texturas.pdf
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Parâmetros de amostragem da textura.
    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Agora enviamos a imagem lida do disco para a GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    GLuint textureunit = g_NumLoadedTextures;
    glActiveTexture(GL_TEXTURE0 + textureunit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindSampler(textureunit, sampler_id);

    stbi_image_free(data);

    g_NumLoadedTextures += 1;
}

// Esta função cria um programa de GPU, o qual contém obrigatoriamente um
// Vertex Shader e um Fragment Shader.
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
    // Criamos um identificador (ID) para este programa de GPU
    GLuint program_id = glCreateProgram();

    // Definição dos dois shaders GLSL que devem ser executados pelo programa
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    // Linkagem dos shaders acima ao programa
    glLinkProgram(program_id);

    // Verificamos se ocorreu algum erro durante a linkagem
    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    // Imprime no terminal qualquer erro de linkagem
    if (linked_ok == GL_FALSE)
    {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos memória para guardar o log de compilação.
        // A chamada "new" em C++ é equivalente ao "malloc()" do C.
        GLchar *log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "\n== End of link log\n";

        // A chamada "delete" em C++ é equivalente ao "free()" do C
        delete[] log;

        fprintf(stderr, "%s", output.c_str());
    }

    // Os "Shader Objects" podem ser marcados para deleção após serem linkados
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    // Retornamos o ID gerado acima
    return program_id;
}

// Definição da função que será chamada sempre que a janela do sistema
// operacional for redimensionada, por consequência alterando o tamanho do
// "framebuffer" (região de memória onde são armazenados os pixels da imagem).
void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    // Indicamos que queremos renderizar em toda região do framebuffer. A
    // função "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".  Essa é a operação de
    // "Screen Mapping" ou "Viewport Mapping" vista em aula ({+ViewportMapping2+}).
    glViewport(0, 0, width, height);

    // Atualizamos também a razão que define a proporção da janela (largura /
    // altura), a qual será utilizada na definição das matrizes de projeção,
    // tal que não ocorra distorções durante o processo de "Screen Mapping"
    // acima, quando NDC é mapeado para coordenadas de pixels. Veja slides 205-215 do documento Aula_09_Projecoes.pdf.
    //
    // O cast para float é necessário pois números inteiros são arredondados ao
    // serem divididos!
    camera->setScreenRatio((float)width / height);
}

// Variáveis globais que armazenam a última posição do cursor do mouse, para
// que possamos calcular quanto que o mouse se movimentou entre dois instantes
// de tempo. Utilizadas no callback CursorPosCallback() abaixo.
double g_LastCursorPosX, g_LastCursorPosY;

// Função callback chamada sempre que o usuário aperta algum dos botões do mouse
void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
        // posição atual do cursor nas variáveis g_LastCursorPosX e
        // g_LastCursorPosY.  Também, setamos a variável
        // g_LeftMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_LeftMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_LeftMouseButtonPressed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
        // posição atual do cursor nas variáveis g_LastCursorPosX e
        // g_LastCursorPosY.  Também, setamos a variável
        // g_RightMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_RightMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_RightMouseButtonPressed = false;
    }

}

// Função callback chamada sempre que o usuário movimentar o cursor do mouse em
// cima da janela OpenGL.
void CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    // Abaixo executamos o seguinte: caso o botão esquerdo do mouse esteja
    // pressionado, computamos quanto que o mouse se movimento desde o último
    // instante de tempo, e usamos esta movimentação para atualizar os
    // parâmetros que definem a posição da câmera dentro da cena virtual.
    // Assim, temos que o usuário consegue controlar a câmera.

    if (g_LeftMouseButtonPressed)
    {
        // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;

        // Atualizamos parâmetros da câmera com os deslocamentos
        g_CameraTheta -= 0.01f * dx;
        g_CameraPhi += 0.01f * dy;

        // Em coordenadas esféricas, o ângulo phi deve ficar entre -pi/2 e +pi/2.
        float phimax = 3.141592f / 2;
        float phimin = -phimax;
        if (g_CameraPhi > phimax)
            g_CameraPhi = phimax;

        if (g_CameraPhi < phimin)
            g_CameraPhi = phimin;
        // para nao flipar a camera
        if (g_CameraPhi >= -0.1 && g_CameraPhi < 0.0)
            g_CameraPhi = -0.1;
        if (g_CameraPhi <= 0.1 && g_CameraPhi > 0.0)
            g_CameraPhi = 0.1;
        // Atualizamos as variáveis globais para armazenar a posição atual do
        // cursor como sendo a última posição conhecida do cursor.
        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }
}

// Função callback chamada sempre que o usuário movimenta a "rodinha" do mouse.
void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera->calculateZoom(yoffset);
}

// Definição da função que será chamada sempre que o usuário pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
    // =================
    // Não modifique este loop! Ele é utilizando para correção automatizada dos
    // laboratórios. Deve ser sempre o primeiro comando desta função KeyCallback().
    for (int i = 0; i < 10; ++i)
        if (key == GLFW_KEY_0 + i && action == GLFW_PRESS && mod == GLFW_MOD_SHIFT)
            std::exit(100 + i);
    // =================

    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // O código abaixo implementa a seguinte lógica:
    //   Se apertar tecla X       então g_AngleX += delta;
    //   Se apertar tecla shift+X então g_AngleX -= delta;
    //   Se apertar tecla Y       então g_AngleY += delta;
    //   Se apertar tecla shift+Y então g_AngleY -= delta;
    //   Se apertar tecla Z       então g_AngleZ += delta;
    //   Se apertar tecla shift+Z então g_AngleZ -= delta;

    float delta = 3.141592 / 16; // 22.5 graus, em radianos.

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        g_AngleX += (mod & GLFW_MOD_SHIFT) ? -delta : delta;
    }

    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        g_AngleY += (mod & GLFW_MOD_SHIFT) ? -delta : delta;
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        g_AngleZ += (mod & GLFW_MOD_SHIFT) ? -delta : delta;
    }

    // Se o usuário apertar a tecla espaço, resetamos os ângulos de Euler para zero.
    if (key == GLFW_KEY_SPACE)
    {
        if (action == GLFW_PRESS)
            tecla_space_pressionada = true;

        else if (action == GLFW_RELEASE)
            tecla_space_pressionada = false;

        else if (action == GLFW_REPEAT)
            ;
    }

    // Se o usuário apertar a tecla P, utilizamos projeção perspectiva.
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        g_UseFreeCamera = !g_UseFreeCamera;
    }

    // Se o usuário apertar a tecla H, fazemos um "toggle" do texto informativo mostrado na tela.
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        g_ShowInfoText = !g_ShowInfoText;
    }

    if (key == GLFW_KEY_W)
    {
        if (action == GLFW_PRESS)
            tecla_W_pressionada = true;

        else if (action == GLFW_RELEASE)
            tecla_W_pressionada = false;

        else if (action == GLFW_REPEAT)
            ;
    }
    if (key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS)
            tecla_D_pressionada = true;

        else if (action == GLFW_RELEASE)
            tecla_D_pressionada = false;

        else if (action == GLFW_REPEAT)
            ;
    }
    if (key == GLFW_KEY_S)
    {
        if (action == GLFW_PRESS)
            tecla_S_pressionada = true;

        else if (action == GLFW_RELEASE)
            tecla_S_pressionada = false;

        else if (action == GLFW_REPEAT)
            ;
    }

    if (key == GLFW_KEY_A)
    {
        if (action == GLFW_PRESS)
            tecla_A_pressionada = true;

        else if (action == GLFW_RELEASE)
            tecla_A_pressionada = false;

        else if (action == GLFW_REPEAT)
            ;
    }
    if (key == GLFW_KEY_J)
    {
        if (action == GLFW_PRESS)
            tecla_J_pressionada = true;

        else if (action == GLFW_RELEASE)
            tecla_J_pressionada = false;

        else if (action == GLFW_REPEAT)
            ;
    }
    if (key == GLFW_KEY_LEFT_CONTROL)
    {
        if (action == GLFW_PRESS)
            tecla_ctrl_pressionada = true;

        else if (action == GLFW_RELEASE)
            tecla_ctrl_pressionada = false;

        else if (action == GLFW_REPEAT)
            ;
    }
}

// Definimos o callback para impressão de erros da GLFW no terminal
void ErrorCallback(int error, const char *description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

// Esta função recebe um vértice com coordenadas de modelo p_model e passa o
// mesmo por todos os sistemas de coordenadas armazenados nas matrizes model,
// view, e projection; e escreve na tela as matrizes e pontos resultantes
// dessas transformações.
void TextRendering_ShowModelViewProjection(
    GLFWwindow *window,
    glm::mat4 projection,
    glm::mat4 view,
    glm::mat4 model,
    glm::vec4 p_model)
{
    if (!g_ShowInfoText)
        return;

    glm::vec4 p_world = model * p_model;
    glm::vec4 p_camera = view * p_world;
    glm::vec4 p_clip = projection * p_camera;
    glm::vec4 p_ndc = p_clip / p_clip.w;

    float pad = TextRendering_LineHeight(window);

    TextRendering_PrintString(window, " Model matrix             Model     In World Coords.", -1.0f, 1.0f - pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, model, p_model, -1.0f, 1.0f - 2 * pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f - 6 * pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f - 7 * pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f - 8 * pad, 1.0f);

    TextRendering_PrintString(window, " View matrix              World     In Camera Coords.", -1.0f, 1.0f - 9 * pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, view, p_world, -1.0f, 1.0f - 10 * pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f - 14 * pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f - 15 * pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f - 16 * pad, 1.0f);

    TextRendering_PrintString(window, " Projection matrix        Camera                    In NDC", -1.0f, 1.0f - 17 * pad, 1.0f);
    TextRendering_PrintMatrixVectorProductDivW(window, projection, p_camera, -1.0f, 1.0f - 18 * pad, 1.0f);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glm::vec2 a = glm::vec2(-1, -1);
    glm::vec2 b = glm::vec2(+1, +1);
    glm::vec2 p = glm::vec2(0, 0);
    glm::vec2 q = glm::vec2(width, height);

    glm::mat4 viewport_mapping = Matrix(
        (q.x - p.x) / (b.x - a.x), 0.0f, 0.0f, (b.x * p.x - a.x * q.x) / (b.x - a.x),
        0.0f, (q.y - p.y) / (b.y - a.y), 0.0f, (b.y * p.y - a.y * q.y) / (b.y - a.y),
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    TextRendering_PrintString(window, "                                                       |  ", -1.0f, 1.0f - 22 * pad, 1.0f);
    TextRendering_PrintString(window, "                            .--------------------------'  ", -1.0f, 1.0f - 23 * pad, 1.0f);
    TextRendering_PrintString(window, "                            V                           ", -1.0f, 1.0f - 24 * pad, 1.0f);

    TextRendering_PrintString(window, " Viewport matrix           NDC      In Pixel Coords.", -1.0f, 1.0f - 25 * pad, 1.0f);
    TextRendering_PrintMatrixVectorProductMoreDigits(window, viewport_mapping, p_ndc, -1.0f, 1.0f - 26 * pad, 1.0f);
}

// Escrevemos na tela os ângulos de Euler definidos nas variáveis globais
// g_AngleX, g_AngleY, e g_AngleZ.
void TextRendering_ShowEulerAngles(GLFWwindow *window)
{
    if (!g_ShowInfoText)
        return;

    float pad = TextRendering_LineHeight(window);

    char buffer[80];
    snprintf(buffer, 80, "Euler Angles rotation matrix = Z(%.2f)*Y(%.2f)*X(%.2f)\n", g_AngleZ, g_AngleY, g_AngleX);

    TextRendering_PrintString(window, buffer, -1.0f + pad / 10, -1.0f + 2 * pad / 10, 1.0f);
}

// Escrevemos na tela qual matriz de projeção está sendo utilizada.
void TextRendering_ShowProjection(GLFWwindow *window)
{
    if (!g_ShowInfoText)
        return;

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    if (g_UsePerspectiveProjection)
        TextRendering_PrintString(window, "Perspective", 1.0f - 13 * charwidth, -1.0f + 2 * lineheight / 10, 1.0f);
    else
        TextRendering_PrintString(window, "Orthographic", 1.0f - 13 * charwidth, -1.0f + 2 * lineheight / 10, 1.0f);
}

// Escrevemos na tela o número de quadros renderizados por segundo (frames per
// second).
void TextRendering_ShowFramesPerSecond(GLFWwindow *window)
{
    if (!g_ShowInfoText)
        return;

    // Variáveis estáticas (static) mantém seus valores entre chamadas
    // subsequentes da função!
    static float old_seconds = (float)glfwGetTime();
    static int ellapsed_frames = 0;
    static char buffer[20] = "?? fps";
    static int numchars = 7;

    ellapsed_frames += 1;

    // Recuperamos o número de segundos que passou desde a execução do programa
    float seconds = (float)glfwGetTime();

    // Número de segundos desde o último cálculo do fps
    float ellapsed_seconds = seconds - old_seconds;

    if (ellapsed_seconds > 1.0f)
    {
        numchars = snprintf(buffer, 20, "%.2f fps", ellapsed_frames / ellapsed_seconds);

        old_seconds = seconds;
        ellapsed_frames = 0;
    }

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    TextRendering_PrintString(window, buffer, 0.0f - (numchars + 1) * charwidth, 0.9f - lineheight, 2.0f);
}

// Função para debugging: imprime no terminal todas informações de um modelo
// geométrico carregado de um arquivo ".obj".
// Veja: https://github.com/syoyo/tinyobjloader/blob/22883def8db9ef1f3ffb9b404318e7dd25fdbb51/loader_example.cc#L98
void PrintObjModelInfo(ObjModel *model)
{
    const tinyobj::attrib_t &attrib = model->attrib;
    const std::vector<tinyobj::shape_t> &shapes = model->shapes;
    const std::vector<tinyobj::material_t> &materials = model->materials;

    printf("# of vertices  : %d\n", (int)(attrib.vertices.size() / 3));
    printf("# of normals   : %d\n", (int)(attrib.normals.size() / 3));
    printf("# of texcoords : %d\n", (int)(attrib.texcoords.size() / 2));
    printf("# of shapes    : %d\n", (int)shapes.size());
    printf("# of materials : %d\n", (int)materials.size());

    for (size_t v = 0; v < attrib.vertices.size() / 3; v++)
    {
        printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
               static_cast<const double>(attrib.vertices[3 * v + 0]),
               static_cast<const double>(attrib.vertices[3 * v + 1]),
               static_cast<const double>(attrib.vertices[3 * v + 2]));
    }

    for (size_t v = 0; v < attrib.normals.size() / 3; v++)
    {
        printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
               static_cast<const double>(attrib.normals[3 * v + 0]),
               static_cast<const double>(attrib.normals[3 * v + 1]),
               static_cast<const double>(attrib.normals[3 * v + 2]));
    }

    for (size_t v = 0; v < attrib.texcoords.size() / 2; v++)
    {
        printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
               static_cast<const double>(attrib.texcoords[2 * v + 0]),
               static_cast<const double>(attrib.texcoords[2 * v + 1]));
    }

    // For each shape
    for (size_t i = 0; i < shapes.size(); i++)
    {
        printf("shape[%ld].name = %s\n", static_cast<long>(i),
               shapes[i].name.c_str());
        printf("Size of shape[%ld].indices: %lu\n", static_cast<long>(i),
               static_cast<unsigned long>(shapes[i].mesh.indices.size()));

        size_t index_offset = 0;

        assert(shapes[i].mesh.num_face_vertices.size() ==
               shapes[i].mesh.material_ids.size());

        printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
               static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

        // For each face
        for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++)
        {
            size_t fnum = shapes[i].mesh.num_face_vertices[f];

            printf("  face[%ld].fnum = %ld\n", static_cast<long>(f),
                   static_cast<unsigned long>(fnum));

            // For each vertex in the face
            for (size_t v = 0; v < fnum; v++)
            {
                tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
                printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
                       static_cast<long>(v), idx.vertex_index, idx.normal_index,
                       idx.texcoord_index);
            }

            printf("  face[%ld].material_id = %d\n", static_cast<long>(f),
                   shapes[i].mesh.material_ids[f]);

            index_offset += fnum;
        }

        printf("shape[%ld].num_tags: %lu\n", static_cast<long>(i),
               static_cast<unsigned long>(shapes[i].mesh.tags.size()));
        for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++)
        {
            printf("  tag[%ld] = %s ", static_cast<long>(t),
                   shapes[i].mesh.tags[t].name.c_str());
            printf(" ints: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j)
            {
                printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
                if (j < (shapes[i].mesh.tags[t].intValues.size() - 1))
                {
                    printf(", ");
                }
            }
            printf("]");

            printf(" floats: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j)
            {
                printf("%f", static_cast<const double>(
                                 shapes[i].mesh.tags[t].floatValues[j]));
                if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1))
                {
                    printf(", ");
                }
            }
            printf("]");

            printf(" strings: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j)
            {
                printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
                if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1))
                {
                    printf(", ");
                }
            }
            printf("]");
            printf("\n");
        }
    }

    for (size_t i = 0; i < materials.size(); i++)
    {
        printf("material[%ld].name = %s\n", static_cast<long>(i),
               materials[i].name.c_str());
        printf("  material.Ka = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].ambient[0]),
               static_cast<const double>(materials[i].ambient[1]),
               static_cast<const double>(materials[i].ambient[2]));
        printf("  material.Kd = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].diffuse[0]),
               static_cast<const double>(materials[i].diffuse[1]),
               static_cast<const double>(materials[i].diffuse[2]));
        printf("  material.Ks = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].specular[0]),
               static_cast<const double>(materials[i].specular[1]),
               static_cast<const double>(materials[i].specular[2]));
        printf("  material.Tr = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].transmittance[0]),
               static_cast<const double>(materials[i].transmittance[1]),
               static_cast<const double>(materials[i].transmittance[2]));
        printf("  material.Ke = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].emission[0]),
               static_cast<const double>(materials[i].emission[1]),
               static_cast<const double>(materials[i].emission[2]));
        printf("  material.Ns = %f\n",
               static_cast<const double>(materials[i].shininess));
        printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
        printf("  material.dissolve = %f\n",
               static_cast<const double>(materials[i].dissolve));
        printf("  material.illum = %d\n", materials[i].illum);
        printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
        printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
        printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
        printf("  material.map_Ns = %s\n",
               materials[i].specular_highlight_texname.c_str());
        printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
        printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
        printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
        printf("  <<PBR>>\n");
        printf("  material.Pr     = %f\n", materials[i].roughness);
        printf("  material.Pm     = %f\n", materials[i].metallic);
        printf("  material.Ps     = %f\n", materials[i].sheen);
        printf("  material.Pc     = %f\n", materials[i].clearcoat_thickness);
        printf("  material.Pcr    = %f\n", materials[i].clearcoat_thickness);
        printf("  material.aniso  = %f\n", materials[i].anisotropy);
        printf("  material.anisor = %f\n", materials[i].anisotropy_rotation);
        printf("  material.map_Ke = %s\n", materials[i].emissive_texname.c_str());
        printf("  material.map_Pr = %s\n", materials[i].roughness_texname.c_str());
        printf("  material.map_Pm = %s\n", materials[i].metallic_texname.c_str());
        printf("  material.map_Ps = %s\n", materials[i].sheen_texname.c_str());
        printf("  material.norm   = %s\n", materials[i].normal_texname.c_str());
        std::map<std::string, std::string>::const_iterator it(
            materials[i].unknown_parameter.begin());
        std::map<std::string, std::string>::const_iterator itEnd(
            materials[i].unknown_parameter.end());

        for (; it != itEnd; it++)
        {
            printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
        }
        printf("\n");
    }
}

// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :

void writeEntities(std::ostream &outputStream, const std::vector<SerializedEntity> &vec)
{
    typename std::vector<SerializedEntity>::size_type size = vec.size();
    outputStream.write((char *)&size, sizeof(size));
    outputStream.write((char *)&vec[0], vec.size() * sizeof(SerializedEntity));
}
std::vector<SerializedEntity> readEntities(std::istream &inputStream)
{

    typename std::vector<SerializedEntity>::size_type size = 0;
    inputStream.read((char *)&size, sizeof(size));
    std::vector<SerializedEntity> allSerialized;
    allSerialized.resize(size);
    inputStream.read((char *)&allSerialized[0], allSerialized.size() * sizeof(SerializedEntity));
    return allSerialized;
}
GLuint buildVisualizationOfBbox(Model model)
{
    glm::vec4 boundingBoxMax = glm::vec4(model.getBboxMax(), 1.0);
    glm::vec4 boundingBoxMin = glm::vec4(model.getBboxMin(), 1.0);
    // boundingBoxMax = entity.getTransformationMatrix() * boundingBoxMax;
    // boundingBoxMin = entity.getTransformationMatrix() * boundingBoxMin;

    GLfloat model_coefficients[] = {
        // 4 linhas o suficiente para mostrar o bounding box i guess

        //       X                 Y                    Z             W
        boundingBoxMax.x, boundingBoxMax.y, boundingBoxMax.z, 1.0f, // posição do vértice 0

        boundingBoxMin.x, boundingBoxMax.y, boundingBoxMax.z, 1.0f, // posição do vértice 1
                                                                    //       X                 Y                    Z             W
        boundingBoxMax.x, boundingBoxMin.y, boundingBoxMax.z, 1.0f, // posição do vértice 2

        boundingBoxMax.x, boundingBoxMax.y, boundingBoxMin.z, 1.0f, // posição do vértice 3

        //       X                 Y                    Z             W
        boundingBoxMin.x, boundingBoxMin.y, boundingBoxMin.z, 1.0f, // posição do vértice 4
                                                                    //       X                 Y                    Z             W
        boundingBoxMax.x, boundingBoxMin.y, boundingBoxMin.z, 1.0f, // posição do vértice 5

        boundingBoxMin.x, boundingBoxMax.y, boundingBoxMin.z, 1.0f, // posição do vértice 6

        boundingBoxMin.x, boundingBoxMin.y, boundingBoxMax.z, 1.0f // posição do vértice 7
    };
    // Criamos o identificador (ID) de um Vertex Buffer Object (VBO).  Um VBO é
    // um buffer de memória que irá conter os valores de um certo atributo de
    // um conjunto de vértices; por exemplo: posição, cor, normais, coordenadas
    // de textura.  Neste exemplo utilizaremos vários VBOs, um para cada tipo de atributo.
    // Agora criamos um VBO para armazenarmos um atributo: posição.
    GLuint VBO_model_coefficients_id;
    glGenBuffers(1, &VBO_model_coefficients_id);

    // Criamos o identificador (ID) de um Vertex Array Object (VAO).  Um VAO
    // contém a definição de vários atributos de um certo conjunto de vértices;
    // isto é, um VAO irá conter ponteiros para vários VBOs.
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);

    // "Ligamos" o VAO ("bind"). Informamos que iremos atualizar o VAO cujo ID
    // está contido na variável "vertex_array_object_id".
    glBindVertexArray(vertex_array_object_id);

    // "Ligamos" o VBO ("bind"). Informamos que o VBO cujo ID está contido na
    // variável VBO_model_coefficients_id será modificado a seguir. A
    // constante "GL_ARRAY_BUFFER" informa que esse buffer é de fato um VBO, e
    // irá conter atributos de vértices.
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);

    // Alocamos memória para o VBO "ligado" acima. Como queremos armazenar
    // nesse VBO todos os valores contidos no array "model_coefficients", pedimos
    // para alocar um número de bytes exatamente igual ao tamanho ("size")
    // desse array. A constante "GL_STATIC_DRAW" dá uma dica para o driver da
    // GPU sobre como utilizaremos os dados do VBO. Neste caso, estamos dizendo
    // que não pretendemos alterar tais dados (são estáticos: "STATIC"), e
    // também dizemos que tais dados serão utilizados para renderizar ou
    // desenhar ("DRAW").  Pense que:
    //
    //            glBufferData()  ==  malloc() do C  ==  new do C++.
    //
    glBufferData(GL_ARRAY_BUFFER, sizeof(model_coefficients), NULL, GL_STATIC_DRAW);

    // Finalmente, copiamos os valores do array model_coefficients para dentro do
    // VBO "ligado" acima.  Pense que:
    //
    //            glBufferSubData()  ==  memcpy() do C.
    //
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(model_coefficients), model_coefficients);

    // Precisamos então informar um índice de "local" ("location"), o qual será
    // utilizado no shader "shader_vertex.glsl" para acessar os valores
    // armazenados no VBO "ligado" acima. Também, informamos a dimensão (número de
    // coeficientes) destes atributos. Como em nosso caso são pontos em coordenadas
    // homogêneas, temos quatro coeficientes por vértice (X,Y,Z,W). Isso define
    // um tipo de dado chamado de "vec4" em "shader_vertex.glsl": um vetor com
    // quatro coeficientes. Finalmente, informamos que os dados estão em ponto
    // flutuante com 32 bits (GL_FLOAT).
    // Esta função também informa que o VBO "ligado" acima em glBindBuffer()
    // está dentro do VAO "ligado" acima por glBindVertexArray().
    // Veja https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object
    GLuint location = 0;            // "(location = 0)" em "shader_vertex.glsl"
    GLint number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);

    // "Ativamos" os atributos. Informamos que os atributos com índice de local
    // definido acima, na variável "location", deve ser utilizado durante o
    // rendering.
    glEnableVertexAttribArray(location);

    // "Desligamos" o VBO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLfloat color_coefficients[] = {

        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 0
        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 1

        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 2
        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 3

        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 4
        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 5

        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 6
        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 7

    };
    GLuint VBO_color_coefficients_id;
    glGenBuffers(1, &VBO_color_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_coefficients), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color_coefficients), color_coefficients);
    location = 1;             // "(location = 1)" em "shader_vertex.glsl"
    number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Vamos então definir polígonos utilizando os vértices do array
    // model_coefficients.
    //
    // Para referência sobre os modos de renderização, veja slides 182-188 do documento Aula_04_Modelagem_Geometrica_3D.pdf.
    //
    // Este vetor "indices" define a TOPOLOGIA (veja slides 103-110 do documento Aula_04_Modelagem_Geometrica_3D.pdf).
    //
    GLuint indices[] = {
        // Definimos os índices dos vértices que definem as ARESTAS de um cubo
        // através de 6 linhas que serão desenhadas com o modo de renderização
        // GL_LINES.
        0, 1, // linha 1
        0, 2, // linha 2
        0, 3, // linha 3
        4, 5, // linha 4
        4, 6, // linha 5
        4, 7, // linha 6
    };
    // Criamos um buffer OpenGL para armazenar os índices acima
    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

    // Alocamos memória para o buffer.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), NULL, GL_STATIC_DRAW);

    // Copiamos os valores do array indices[] para dentro do buffer.
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

    // NÃO faça a chamada abaixo! Diferente de um VBO (GL_ARRAY_BUFFER), um
    // array de índices (GL_ELEMENT_ARRAY_BUFFER) não pode ser "desligado",
    // caso contrário o VAO irá perder a informação sobre os índices.
    //
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // XXX Errado!
    //

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);

    // Retornamos o ID do VAO. Isso é tudo que será necessário para renderizar
    // os triângulos definidos acima. Veja a chamada glDrawElements() em main().
    return vertex_array_object_id;
}

void showCollisionBoxes(std::vector<Entity> walls, Entity kart, GLuint kartVAO, GLuint wallVAO, GLuint g_GpuProgramID)
{
    int numberOfWalls = walls.size();
    for (int i = 0; i < numberOfWalls; i++)
    {
        glLineWidth(10);
        GLint model_uniform = glGetUniformLocation(g_GpuProgramID, "model");
        glBindVertexArray(wallVAO);
        glm::mat4 modelA = walls[i].getTransformationMatrix();
        glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(modelA));
        glDrawElements(
            GL_LINES,
            12,
            GL_UNSIGNED_INT,
            (void *)0);
        glBindVertexArray(0);
    }

    GLint model_uniform = glGetUniformLocation(g_GpuProgramID, "model");
    glBindVertexArray(kartVAO);
    glm::mat4 modelB = kart.getTransformationMatrix();
    glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(modelB));
    glDrawElements(
        GL_LINES,
        12,
        GL_UNSIGNED_INT,
        (void *)0);
    glBindVertexArray(0);
}
