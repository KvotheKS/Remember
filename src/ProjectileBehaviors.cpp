#include "ProjectileBehaviors.h"
#include "Sprite.h"
#include "Projectile.h"
#include <cmath>


InvertOnConnection::InvertOnConnection(GameObject& associated, GameObject* target)
    : GameObject(associated), target(target)
{}

void InvertOnConnection::Update(float dt)
{
    if(associated.box.IsInside(Vec2(target->box.x,target->box.y)) 
        || target->box.IsInside(Vec2(associated.box.x, associated.box.y)))
    {
        auto spr = (Sprite*)associated.GetComponent(C_ID::Sprite);
        auto proj = (Projectile*)associated.GetComponent(C_ID::Projectile);
        spr->SetFliped(!spr->fliped);
        proj->velocity.Rotate(M_PI);
        associated.RemoveComponent(this);
    }
}