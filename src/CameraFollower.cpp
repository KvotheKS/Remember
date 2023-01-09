#include "CameraFollower.h"

CameraFollower::CameraFollower(GameObject& go) : GameObject(go){
}

void CameraFollower::Update(float dt){
    associated.box.x = Camera::pos.x;
    associated.box.y = Camera::pos.y;
}

void CameraFollower::Render(){
}

bool CameraFollower::Is(std::string type){
    return type == "CameraFollower";
}

bool CameraFollower::Is(C_ID type)
{ return type == C_ID::CameraFollower; }
