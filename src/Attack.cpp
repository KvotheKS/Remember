#include "Attack.h"

Attack::Attack(GameObject& associated, int damage, Vec2 knockback)
    : GameObject(associated)
{
    this->damage = damage;
    this->knockback = knockback;   
}

int Attack::GetDamage() { return damage; }

Vec2 Attack::GetKnockback() { return knockback; }

bool Attack::Is(C_ID type)
{ return type == C_ID::Attack; }

DisappearOnHit::DisappearOnHit(GameObject& associated)
    : GameObject(associated)
{}

void DisappearOnHit::NotifyCollision(GameObject& other, Vec2 sep)
{ associated.RequestDelete(); }