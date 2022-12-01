#pragma once
#include "Component.h"
#include "GameObject.h"
#include "TileSet.h"
#include "Camera.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class TileMap : public GameObject {
    private:
        std::vector<int> tileMatrix;
        TileSet* tileSet;
        int mapWidth;
        int mapHeight;
        int mapDepth;
    public:
        TileMap(GameObject& associated, std::string file, TileSet* tileSet);
        void Load(std::string file);
        void SetTileSet(TileSet* tileSet);
        int& At(int x, int y, int z = 0);
        void Update(float dt);
        void Render();
        void Print(float x = -1, float y = -1);
        bool Is(std::string type);
        void RenderLayer(int layer, int cameraX = 0, int cameraY = 0);
        int GetWidth();
        int GetHeight();
        int GetDepth();
};