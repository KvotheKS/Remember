#include "Gate.h"
#include "RigidBody.h"
#include "Player.h"
#include <iostream>



Gate::Gate(GameObject& associated, bool active):GameObject(associated){
    
    associated.box.w = 120;
    associated.box.h = 240;
    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
  
}
Gate::~Gate(){

}

void Gate::Start(){

}
void Gate::Update(float dt){
   
}
void Gate::Render(){

}
bool Gate::Is(std::string type){
    return type == "Gate";
}

bool Gate::Is(C_ID type)
{ return type == C_ID::Gate; }

void Gate::NotifyCollision(GameObject& other,Vec2 sep){
    Player * body = (Player*)other.GetComponent(C_ID::Player);
    if(body == nullptr){
        cout << "ouchmama\n";
    }

}




