#pragma once

#include "GameObject.h"

class Fox : public GameObject
{

public:
    Fox(GameObject&);
    void Update(float);
};