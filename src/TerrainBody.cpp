#include "TerrainBody.h"
#include "RigidBody.h"
#include <iostream>



TerrainBody::TerrainBody(GameObject& associated, bool hascpts):GameObject(associated){
    if(hascpts)
    {
        
        Collider* collider = new Collider(associated);
        associated.AddComponent(collider);
    }
    
}
TerrainBody::~TerrainBody(){

}

void TerrainBody::Start(){

}
void TerrainBody::Update(float dt){
   
}
void TerrainBody::Render(){

}
bool TerrainBody::Is(std::string type){
    return type == "TerrainBody";
}

bool TerrainBody::Is(C_ID type)
{ return type == C_ID::TerrainBody; }

void TerrainBody::NotifyCollision(GameObject& other,Vec2 sep){
    
    // if(RigidBody * body = (RigidBody*)other.GetComponent("RigidBody")){
        
    // }

}




