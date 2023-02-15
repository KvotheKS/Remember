#include "Gate.h"
#include "RigidBody.h"
#include "Player.h"
#include "Game.h"
#include <iostream>



Gate::Gate(GameObject& associated, State * target_state, int spawn_point, bool active):GameObject(associated){
    
    associated.box.w = 120;
    associated.box.h = 240;
    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    this->target_state = target_state;
    this->spawn_point = spawn_point;
    this->active = active;
  
}
Gate::~Gate(){

}

void Gate::Start(){

}
void Gate::Update(float dt){
    if(exitTimer.Update(dt)){   
        Game::GetInstance().GetCurrentState().popRequested = true;
        Game::GetInstance().Push(target_state);
    }
}
void Gate::Render(){

}
bool Gate::Is(std::string type){
    return type == "Gate";
}

bool Gate::Is(C_ID type)
{ return type == C_ID::Gate; }

void Gate::NotifyCollision(GameObject& other,Vec2 sep){
    if(!active)return;
    Player * player = (Player*)other.GetComponent(C_ID::Player);
    if(player != nullptr && player->pause == false){
        GameData::spawn_ID = spawn_point;
        exitTimer.SetFinish(0.8);
        exitTimer.Restart();
        player->SetPause(true);
        GameObject* GO_fade = new GameObject();
            GO_fade->depth = 1000;
            float fadeinTime = 0.8; float fadeoutTime = 0;float duration = 1;
            ScreenFade* effect = new ScreenFade(*GO_fade, fadeinTime, fadeoutTime, duration);
            
            GO_fade->AddComponent(effect);
        
            State& state = Game::GetInstance().GetCurrentState();
        state.AddObject(GO_fade);
 
    } 
}




