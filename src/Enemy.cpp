#include "Enemy.h"
#include "IA.h"
#include "Sprite.h"
#include "Game.h"
#include "State.h"

Enemy::Enemy(GameObject& associated)
    : GameObject(associated)
{
    IA* brainz = new IA(associated, GetTarget(), 1.0f);
    
    brainz->AddAction(
        {
            {Vec2(30,0), 1, 3}, // CLOSE
            {Vec2(100,0), 1, 2}, // MEDIUM
            {Vec2(200,0), 2, 3} // FAR
        }
    );
    associated.AddComponent(brainz);
}

GameObject* Enemy::GetTarget()
{
    return Game::GetInstance()
    .GetCurrentState()
    .GetObject(C_ID::Player)
    .lock()
    .get();
}

void Enemy::Update(float dt)
{
    IA* brainz = (IA*)associated.GetComponent(C_ID::IA);
    if(!brainz->target) brainz->target = GetTarget();
    State& st = Game::GetInstance().GetCurrentState();
    if(brainz->selectedAction == -1) return;
    
    GameObject* bullGO = new GameObject;
    Sprite* spr; 
    switch (brainz->selectedAction)
    {
        case CLOSE:
            spr = new Sprite(*bullGO, "assets/img/Zidle.png",1,0.0f,10);
            bullGO->box =associated.box +  Rect(30,0,0,0);
        break;
        case MEDIUM:
            spr = new Sprite(*bullGO, "assets/img/Zcrouch.png",1,0.0f,10);
            bullGO->box = associated.box + Rect(100,0,0,0);
        break;
        case FAR:
            spr = new Sprite(*bullGO, "assets/img/ZFall.png",1,0.0f,10);
            bullGO->box = associated.box + Rect(150,0,0,0);
        break;
    }
    bullGO->AddComponent(spr);
    st.AddObject(bullGO);    

}

bool Enemy::Is(C_ID type)
{ return type == C_ID::Enemy; }