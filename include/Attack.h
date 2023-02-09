#pragma once

#include "GameObject.h"

class Attack : public GameObject
{
public:
    int damage;
    Vec2 knockback;
    GameObject* owner;
public:
    Attack(GameObject&, int = 0, Vec2 = Vec2(0,0), GameObject* = nullptr);
    virtual int GetDamage();
    virtual Vec2 GetKnockback();
    virtual bool OwnedBy(GameObject*);
    bool Is(C_ID);
};

class DisappearOnHit : public GameObject
{
    GameObject* owner;
public:
    DisappearOnHit(GameObject&, GameObject*);
    void NotifyCollision(GameObject&, Vec2);
};

class DisappearOnDeadOwner : public GameObject
{
    std::weak_ptr<GameObject> owner;
public:
    DisappearOnDeadOwner(GameObject&, std::weak_ptr<GameObject>);
    void Update(float);
};