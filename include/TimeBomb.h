#pragma once

#include "GameObject.h"
#include "Timer.h"
#include <functional>

class TimeBomb : public GameObject
{
    bool rendered;
    Timer tm;
public:
    TimeBomb(GameObject&, float);
    void Update(float);
    void Render();
    bool Is(C_ID);
};

