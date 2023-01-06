#include "TileMap.h"
#include "Scheduler.h"

TileMap::TileMap(GameObject& associated, std::string file, TileSet* tileSet) : GameObject(associated){
    Load(file);
    this->tileSet = tileSet;
}

void TileMap::Load(std::string file){
    std::ifstream inFile(file, std::ios::in);
    if(!inFile){
        std::cerr << "Unable to open " << file << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string line, value;
    getline(inFile, line);
    std::stringstream sline(line);

    getline(sline, value, ',');
    mapWidth = stoi(value);

    getline(sline, value, ',');
    mapHeight = stoi(value);

    getline(sline, value, ',');
    mapDepth = stoi(value);

    while(getline(inFile, line)){
        std::stringstream sline(line);
        while(getline(sline, value, ','))
            tileMatrix.push_back(stoi(value) - 1);
    }

    inFile.close();
}

void TileMap::SetTileSet(TileSet* tileSet){
    this->tileSet = tileSet;
}

int& TileMap::At(int x, int y, int z){
    int index = x + mapWidth * (y + mapHeight * z);
    return tileMatrix[index];
}

void TileMap::Update(float dt){
}

void TileMap::Render(){
    for(int i = 0; i < mapDepth; i++)
        Scheduler::Push(this, associated.depth + i, i);
}

void TileMap::Print(float x, float y)
{
    float layerSpeed = 1.0 + 0.25 * x;
    // RenderLayer(x, Camera::pos.x * layerSpeed, Camera::pos.y * layerSpeed);
    RenderLayer(x, Camera::pos.x , Camera::pos.y );
    
}

bool TileMap::Is(std::string type){
    return type == "TileMap";
}

void TileMap::RenderLayer(int layer, int cameraX, int cameraY){
    for(int i = 0; i < mapHeight; i++){
        for(int j = 0; j < mapWidth; j++){
            int tileX = j * tileSet->GetTileWidth() - cameraX;
            int tileY = i * tileSet->GetTileHeight() - cameraY;
            tileSet->RenderTile(At(j, i, layer), tileX, tileY);
        }
    }
}

int TileMap::GetWidth(){
    return mapWidth;
}

int TileMap::GetHeight(){
    return mapHeight;
}

int TileMap::GetDepth(){
    return mapDepth;
}