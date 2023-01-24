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
    //terreno na posicao dos tiles com ID 19
    for(int i = 0; i < mapHeight; i++){
        
        for(int j = 0; j < mapWidth; j++){

            
            int tileX = j * tileSet->GetTileWidth() ;
            int tileY = i * tileSet->GetTileHeight() ;
            
            // para printar numero das tiles
            
            // GameObject* goText = new GameObject();
            //     goText->depth =1000;
            //     std::string file = "assets/font/Call me maybe.ttf";
            //     int temp = tileMatrix[j+i*mapWidth];
            //     std::string msg = ".";
            //     if(temp != -1){
            //         msg = to_string(temp);
                    
            //     }
            //     Text* text = new Text(*goText, file, 35, Text::TextStyle::BLENDED, msg, {0, 100, 255, 255}, 1);
            //     goText->AddComponent(text);

               
                
            //     goText->box.SetCenter(tileX+30, tileY+30);
            // state.objectArray.emplace_back(goText);

            
            


            if (tileMatrix[j+i*mapWidth] == 19){//!!idealmente temos que organizar essas alternativas
                
                
            
                GameObject* terrainbox = new GameObject();
                    terrainbox->depth =999;

                    Collider* collider = new Collider(*terrainbox);
                    terrainbox->AddComponent(collider);

                    terrainbox->box.x = tileX;
                    terrainbox->box.y = tileY;
                    terrainbox->box.h = tileSet->GetTileHeight()*2;
                    terrainbox->box.w = tileSet->GetTileWidth();
                  
                    terrainbox->angleDeg = 0;
                state.terrainArray.emplace_back(terrainbox);
            }  
            if (tileMatrix[j+i*mapWidth] == 81 ){//45° pra esquerda
                

                GameObject* terrainbox = new GameObject();
                    terrainbox->depth =999;

                    Collider* collider = new Collider(*terrainbox);
                    terrainbox->AddComponent(collider);

                    // terrainbox->box.x = tileX+13;
                    // terrainbox->box.y = tileY+30;
                    // terrainbox->box.h = tileSet->GetTileHeight()*sqrt(2);
                    // terrainbox->box.w = tileSet->GetTileWidth()*2;

                    terrainbox->box.x = tileX+18;
                    terrainbox->box.y = tileY+18;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(2);
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(2);
                  
                    terrainbox->angleDeg = 45;
                    
                state.terrainArray.emplace_back(terrainbox);
            } 
            if (tileMatrix[j+i*mapWidth] == 83 ){//45° pra direita
                

                GameObject* terrainbox = new GameObject();
                    terrainbox->depth =999;

                    Collider* collider = new Collider(*terrainbox);
                    terrainbox->AddComponent(collider);

            

                    terrainbox->box.x = tileX-42;
                    terrainbox->box.y = tileY+18;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(2);
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(2);
                  
                    terrainbox->angleDeg = +45;
                state.terrainArray.emplace_back(terrainbox);
            } 
            if (tileMatrix[j+i*mapWidth] == 26 ){//22.5° pra direita primeira metada
                

                GameObject* terrainbox = new GameObject();
                    terrainbox->depth =999;

                    Collider* collider = new Collider(*terrainbox);
                    terrainbox->AddComponent(collider);

            

                    terrainbox->box.x = tileX+13;
                    terrainbox->box.y = tileY+41;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45+19;
                state.terrainArray.emplace_back(terrainbox);
            }  
            if (tileMatrix[j+i*mapWidth] == 27 ){//22.5° pra direita segunda metade
                

                GameObject* terrainbox = new GameObject();
                    terrainbox->depth =999;

                    Collider* collider = new Collider(*terrainbox);
                    terrainbox->AddComponent(collider);

            

                    terrainbox->box.x = tileX+13;
                    terrainbox->box.y = tileY+11;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45+19;
                state.terrainArray.emplace_back(terrainbox);
            }  
            if (tileMatrix[j+i*mapWidth] == 87 ){//22.5° pra esquerda segunda metade
                

                GameObject* terrainbox = new GameObject();
                    terrainbox->depth =999;

                    Collider* collider = new Collider(*terrainbox);
                    terrainbox->AddComponent(collider);

            

                    terrainbox->box.x = tileX-17;
                    terrainbox->box.y = tileY+11;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45-19;
                state.terrainArray.emplace_back(terrainbox);
            }  
             if (tileMatrix[j+i*mapWidth] == 88 ){//22.5° pra esquerda segunda metade
                

                GameObject* terrainbox = new GameObject();
                    terrainbox->depth =999;

                    Collider* collider = new Collider(*terrainbox);
                    terrainbox->AddComponent(collider);

            

                    terrainbox->box.x = tileX-17;
                    terrainbox->box.y = tileY+41;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45-19;
                state.terrainArray.emplace_back(terrainbox);
            }                                      
        }
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