#include "TileSet.h"

TileSet::TileSet(int tileWidth, int tileHeight, std::string file) : tileSet(*new GameObject(), file){
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;

    if(tileSet.IsOpen()){
        rows = tileSet.GetHeight() / tileHeight;
        columns = tileSet.GetWidth() / tileWidth;
    }
}

void TileSet::RenderTile(unsigned index, float x, float y, float depth){
    if(index < (unsigned) rows*columns){
        int clipX = (index % columns) * tileWidth;
        int clipY = (index / columns) * tileHeight;
        tileSet.SetClip(clipX, clipY, tileWidth, tileHeight);

        tileSet.Render(x, y, depth);
    }
}

int TileSet::GetTileWidth(){
    return tileWidth;
}

int TileSet::GetTileHeight(){
    return tileHeight;
}