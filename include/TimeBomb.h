#pragma once

#include "GameObject.h"
#include "Timer.h"
#include <functional>

class TimeBomb : public GameObject
{
    Timer tm;
public:
    TimeBomb(GameObject&, float);
    void Update(float);
    bool Is(C_ID);
};

