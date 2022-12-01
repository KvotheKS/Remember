#include "RigidBody.h"


RigidBody::RigidBody(GameObject& associated, int modo):GameObject(associated), modo(modo){
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
void RigidBody::NotifyCollision(GameObject& other){
   
    if(RigidBody * body = (RigidBody*)other.GetComponent("RigidBody")){
        if(body->modo == 1){
            printf("ouch\n");
            
        }
        else{

        }
    }
    
}