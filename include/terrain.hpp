#pragma once
#include "sceneObject.hpp"
#include "VAO.hpp"

class Terrain{

    public:
        Terrain();
        SceneObject generateTerrain(VAO vao);
    private:
        SceneObject object;
        const int SIZE = 256;
        const int VERTEX = 64;
};