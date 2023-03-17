#pragma once
#include "model.hpp"
#include "VAO.hpp"

class Terrain{

    public:
        Terrain();
        Model generateTerrain(VAO vao);
    private:
        Model object;
        const int SIZE = 256;
        const int VERTEX = 64;
};