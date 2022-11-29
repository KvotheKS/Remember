#include "RigidBody.h"
#include <iostream>

// define pra test -m
using namespace std;
#define p(x) cout << #x << ": " << x << endl;

RigidBody::RigidBody(GameObject& associated, int modo):Component(associated), modo(modo){
    Sprite* pbody = new Sprite(associated, "assets/img/testblock.jpg");
    associated.AddComponent(pbody);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
}
RigidBody::~RigidBody(){

}

void RigidBody::Start(){

}
void RigidBody::Update(float dt){
    InputManager& inManager = InputManager::GetInstance();
    if(inManager.MousePress(LEFT_MOUSE_BUTTON) && modo == 1){
        associated.box.x = inManager.GetMouseX() + Camera::pos.x;
        associated.box.y = inManager.GetMouseY() + Camera::pos.y;
    }
}
void RigidBody::Render(){

}
bool RigidBody::Is(std::string type){
    return type == "RigidBody";
}
void RigidBody::NotifyCollision(GameObject& other,Vec2 sep){
    
    if(RigidBody * body = (RigidBody*)other.GetComponent("RigidBody")){
        if(body->modo == 1){
            // printf("ouch\n");
            if(other.box.GetCenter().x < associated.box.GetCenter().x){
                associated.box.x += sep.x;
            }else{
                associated.box.x -= sep.x;
            }
            
            if(other.box.GetCenter().y > associated.box.GetCenter().y){
                associated.box.y += sep.y;
            }else{
                associated.box.y -= sep.y;
            }

            if(Collider* collider = (Collider*)associated.GetComponent("Collider")){
            collider->Update(0);
    }
            
        }
        else{
           
        }
    }
    
}