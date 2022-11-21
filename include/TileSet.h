#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include <string>

class TileSet {
    private:
        Sprite tileSet;
        int rows;
        int columns;
        int tileWidth;
        int tileHeight;
    public:
        TileSet(int tileWidth, int tileHeight, std::string file);
        void RenderTile(unsigned index, float x, float y, float depth = 0.0f);
        int GetTileWidth();
        int GetTileHeight();
};