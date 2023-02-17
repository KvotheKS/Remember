#pragma once

#include "GameObject.h"

class InvertOnConnection : public GameObject
{
    GameObject* target;
public:
    InvertOnConnection(GameObject&, GameObject*);
    void Update(float);
};