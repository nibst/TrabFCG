#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;
in vec4 color_vertex;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
#define SPHERE 0
#define KART  1
#define PLANE  2
#define OUTERWALL 3
#define INNERWALL 4
#define COW 5
#define FINISH 6
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;
uniform sampler2D TextureImage5;
uniform sampler2D TextureImage6;
uniform sampler2D TextureImage7;
uniform sampler2D TextureImage8;






// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;



    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,1.0,0.0));

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor que define o sentido da reflexão especular ideal.
    vec4 r = -l + 2 * n * dot(n,l);

    // Parâmetros que definem as propriedades espectrais da superfície
    vec3 Ks; // Refletância especular
    vec3 Ka; // Refletância ambiente
    float q; // Expoente especular para o modelo de iluminação de Phong

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;
    vec3 Kd0;

    if ( object_id == KART )
    {
        Ks = vec3(0.8, 0.8, 0.8);
        Ka = Ks/8;
        q = 100.0;
        //use texcoords from obj model
        Kd0 =texture(TextureImage3, texcoords).rgb;
    }
    // else if ( object_id == OUTERWALL ){

    //     //Computa a cor da textura neste ponto
    //     Kd0 = texture(TextureImage4, texcoords*5).rgb;
    //     Ks = vec3(0.0,0.0,0.0);
    //     Ka = vec3(0.1,0.1,0.1);
    //     q = 2.0;
    // }
    else if (object_id == INNERWALL){
        float adjustObjectBrightness = 3.0;
        Kd0 = texture(TextureImage6, texcoords*3).rgb * adjustObjectBrightness;
        Ks = vec3(0.0,0.0,0.0);
        Ka = vec3(0.1,0.1,0.1);
        q = 0.0;
    }
    else if(object_id == COW){
        Ks = vec3(0.0,0.0,0.0);
        Ka = vec3(0.0,0.0,0.0);
        q = 1.0;

        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;
        float radius = 3.0;
        vec4 plinha = bbox_center + radius*((position_model - bbox_center)/(length(position_model - bbox_center)));
        vec4 vec_p = plinha - bbox_center;
        float theta = atan(vec_p.x,vec_p.z);
        float phi = asin(vec_p.y/radius);

        U = (theta + M_PI)/(2*M_PI);
        V = (phi + M_PI_2)/M_PI;
        Kd0 = texture(TextureImage7, vec2(U,V)).rgb;
    }
    else if ( object_id == FINISH ){
        Ks = vec3(0.6, 0.6, 0.6);
        Ka = Ks/8;
        q = 22.0;
        //use texcoords from obj model
        Kd0 =texture(TextureImage8, texcoords).rgb;
    }
    else if ( object_id == PLANE )
    {
        Ks = vec3(0.0,0.0,0.0);
        Ka = vec3(0.0,0.0,0.0);
        q = 100.0;
        vec2 tiledCoords = texcoords*50;

        //colour of blend map on coord texcoords
        vec4 blendMapColour = texture(TextureImage2,texcoords);

        //in blendMap the background texture is black colour
        float backGroundTextureAmount = 1.0 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
        vec4 backGroundTextureColour = texture(TextureImage0,tiledCoords) * backGroundTextureAmount;
        vec4 blueTextureColour = texture(TextureImage1,tiledCoords) * blendMapColour.b;
        float boostRedTextureColour = 3.0;
        vec4 redTextureColour = texture(TextureImage5,tiledCoords) * blendMapColour.r*boostRedTextureColour;
        Kd0 = backGroundTextureColour.rgb + blueTextureColour.rgb + redTextureColour.rgb;
    }


    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0,1.0,1.0);

    // Equação de Iluminação
    float lambert = max(0,dot(n,l));
    vec4 h = l + v;
    h = h/length(h);

    vec3 blinnPhong =  Ks*I*pow(max(0,dot(n,h)),q);


    // Espectro da luz ambiente
    vec3 Ia = vec3(0.1,0.1,0.1);

    color.rgb = Kd0 * I *(lambert + 0.01) + Ka*Ia + blinnPhong;
    color.a = 1;
    if (object_id == OUTERWALL){
        color.rgb = pow(color_vertex.rgb, vec3(1.0,1.0,1.0)/2.2);
    } else{
        color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
    }
}


