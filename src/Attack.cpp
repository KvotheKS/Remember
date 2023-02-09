#include "Attack.h"

Attack::Attack(GameObject& associated, int damage, Vec2 knockback, GameObject* owner)
    : GameObject(associated)
{
    this->damage = damage;
    this->knockback = knockback;   
    this->owner = owner;
}

int Attack::GetDamage() { return damage; }

Vec2 Attack::GetKnockback() { return knockback; }

bool Attack::OwnedBy(GameObject* isOwner)
{ return isOwner == owner; }

bool Attack::Is(C_ID type)
{ return type == C_ID::Attack; }

DisappearOnHit::DisappearOnHit(GameObject& associated, GameObject* owner)
    : GameObject(associated), owner(owner)
{}

void DisappearOnHit::NotifyCollision(GameObject& other, Vec2 sep)
{ if(&other != owner) associated.RequestDelete(); }

DisappearOnDeadOwner::DisappearOnDeadOwner(GameObject& associated, std::weak_ptr<GameObject> owner)
    : GameObject(associated), owner(owner)
{}

void DisappearOnDeadOwner::Update(float dt)
{ if(owner.expired() || owner.lock()->IsDead()) associated.RequestDelete(); }