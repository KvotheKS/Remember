#include "TerrainBody.h"
#include <iostream>

// define pra test -m
using namespace std;
#define p(x) cout << #x << ": " << x << endl;

TerrainBody::TerrainBody(GameObject& associated):Component(associated){
    Sprite* pbody = new Sprite(associated, "assets/img/testterrain.png");
    associated.AddComponent(pbody);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
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
void TerrainBody::NotifyCollision(GameObject& other,Vec2 sep){
    if(RigidBody * body = (RigidBody*)other.GetComponent("RigidBody")){
        
    }

    
    
}