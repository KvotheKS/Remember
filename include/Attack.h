#pragma once

#include "GameObject.h"

class Attack : public GameObject
{
public:
    int damage;
    Vec2 knockback;

public:
    Attack(GameObject&, int = 0, Vec2 = Vec2(0,0));
    virtual int GetDamage();
    virtual Vec2 GetKnockback();
    bool Is(C_ID);
};

class DisappearOnHit : public GameObject
{
public:
    DisappearOnHit(GameObject&);
    void NotifyCollision(GameObject&, Vec2);
};