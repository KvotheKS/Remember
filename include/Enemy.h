#pragma once

#include "GameObject.h"

class Enemy : public GameObject
{
    enum {CLOSE = 0, MEDIUM, FAR};
public:
    Enemy(GameObject&);
    void Update(float);
    bool Is(C_ID);
    GameObject* GetTarget();
};