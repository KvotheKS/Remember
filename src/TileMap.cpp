#include "TileMap.h"
#include "Scheduler.h"
#include "State.h"
#include "Game.h"
#include "TerrainBody.h"

TileMap::TileMap(GameObject& associated, std::string file, TileSet* tileSet) : GameObject(associated){
    
    this->tileSet = tileSet;
    Load(file);
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


    State& state = Game::GetInstance().GetCurrentState();
    //terreno na posicao dos tiles com ID 16
    // for(int i = 0; i < mapHeight; i++){
        
    //     for(int j = 0; j < mapWidth; j++){
           
    //         //if (At(j, i, 0) == 16){     //?? At() n funciona aqui...por
    //         if (tileMatrix[j+i*mapWidth]== 16){
    //             int tileX = j * tileSet->GetTileWidth() ;
    //             int tileY = i * tileSet->GetTileHeight() ;
                
            
    //             GameObject* terrainbox = new GameObject();
    //                 terrainbox->depth =999;
    //                 TerrainBody* box2 = new TerrainBody(*terrainbox);
    //                 terrainbox->AddComponent(box2);
    //                 terrainbox->box.SetCenter(tileX,tileY);
    //                 terrainbox->angleDeg = 0;
    //             state.AddObject(terrainbox);
    //         }
            
           
    //     }
  
        
    // }

    // !!para testar limites da colisão, delete me
    // for(int i = 0; i < 20;i++){
    //     GameObject* terrainbox = new GameObject();
    //         terrainbox->depth =999;
    //         TerrainBody* box2 = new TerrainBody(*terrainbox);
    //         terrainbox->AddComponent(box2);
    //         terrainbox->box.SetCenter(i*10,0);
    //         terrainbox->angleDeg = 0;
    //     state.AddObject(terrainbox);
    // }
   

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
    RenderLayer(x, Camera::pos.x * layerSpeed, Camera::pos.y * layerSpeed);
    
}

bool TileMap::Is(std::string type){
    return type == "TileMap";
}

bool TileMap::Is(C_ID type)
{ return type == C_ID::TileMap; }

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