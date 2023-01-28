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
            
            // para printar numero das tiles na tela
            
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

            GameObject* terrainbox = new GameObject();
            terrainbox->depth = 1;
            Collider* collider = new Collider(*terrainbox);
            

            switch(tileMatrix[j+i*mapWidth]){
                case(16):case(20):case(18):case(50):case(48):case(46):case(78)://block platform
                    terrainbox->box.x = tileX;
                    terrainbox->box.y = tileY;
                    terrainbox->box.h = tileSet->GetTileHeight();
                    terrainbox->box.w = tileSet->GetTileWidth();
                
                    terrainbox->angleDeg = 0;
                break;
                case(33)://deep wall
                    terrainbox->box.x = tileX;
                    terrainbox->box.y = tileY;
                    terrainbox->box.h = tileSet->GetTileHeight();
                    terrainbox->box.w = tileSet->GetTileWidth()*2;
                
                    terrainbox->angleDeg = 0;
                break;
                case(35)://deep wall
                    terrainbox->box.x = tileX-tileSet->GetTileWidth();
                    terrainbox->box.y = tileY;
                    terrainbox->box.h = tileSet->GetTileHeight();
                    terrainbox->box.w = tileSet->GetTileWidth()*2;
                
                    terrainbox->angleDeg = 0;
                break;
                case(49)://deep floor
                    terrainbox->box.x = tileX;
                    terrainbox->box.y = tileY-tileSet->GetTileHeight();
                    terrainbox->box.h = tileSet->GetTileHeight()*2;
                    terrainbox->box.w = tileSet->GetTileWidth();
                
                    terrainbox->angleDeg = 0;
                break;
                
                case(19): // deep platforms
                    

                    terrainbox->box.x = tileX;
                    terrainbox->box.y = tileY;
                    terrainbox->box.h = tileSet->GetTileHeight()*2;
                    terrainbox->box.w = tileSet->GetTileWidth();
                
                    terrainbox->angleDeg = 0;
                    
                break;
                case(82)://45° pra esquerda
                    terrainbox->box.x = tileX+18;
                    terrainbox->box.y = tileY+18;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(2);
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(2);
                  
                    terrainbox->angleDeg = 45;               
                break;
                case(112)://45° pra esquerda teto
                    terrainbox->box.x = tileX+18;
                    terrainbox->box.y = tileY-44;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(2);
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(2);
                  
                    terrainbox->angleDeg = 45;               
                break;
                case(84)://45° pra direita
                 
                    terrainbox->box.x = tileX-42;
                    terrainbox->box.y = tileY+18;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(2);
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(2);
                  
                    terrainbox->angleDeg = -45;
                        
                break;
                case(114)://45° pra direita teto
                 
                    terrainbox->box.x = tileX-42;
                    terrainbox->box.y = tileY-44;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(2);
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(2);
                  
                    terrainbox->angleDeg = -45;
                        
                break;

                case(26)://22.5° pra direita primeira metada
                    terrainbox->box.x = tileX+12;
                    terrainbox->box.y = tileY+41;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45+19;
                break;

                case(116)://22.5° pra direita primeira metada teto
                    terrainbox->box.x = tileX+12;
                    terrainbox->box.y = tileY-48;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45-19;
                break;

                case(27)://22.5° pra direita segunda metade
                    terrainbox->box.x = tileX+13;
                    terrainbox->box.y = tileY+11;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45+19;
                break;
                case(117)://22.5° pra direita segunda metade teto
                    terrainbox->box.x = tileX+13;
                    terrainbox->box.y = tileY-18;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45-19;
                break;

                case(87)://22.5° pra esquerda primeira metade
                    terrainbox->box.x = tileX-17;
                    terrainbox->box.y = tileY+11;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45-19;
                break;

                case(57)://22.5° pra esquerda primeira metade teto
                    terrainbox->box.x = tileX-17;
                    terrainbox->box.y = tileY-18;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45+19;
                  
                 
                break;

                case(88)://22.5° pra esquerda segunda metade
                    terrainbox->box.x = tileX-17;
                    terrainbox->box.y = tileY+41;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45-19;
                break;

                case(58)://22.5° pra esquerda segunda metade teto
                    terrainbox->box.x = tileX-17;
                    terrainbox->box.y = tileY-49;
                    terrainbox->box.h = tileSet->GetTileHeight()*sqrt(5)/2;
                    terrainbox->box.w = tileSet->GetTileWidth()*sqrt(5)/2;
                  
                    terrainbox->angleDeg = 45+19;
                break;
            }

            terrainbox->AddComponent(collider);
            state.terrainArray.emplace_back(terrainbox);
         
                                      
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