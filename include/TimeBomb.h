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
};

typedef void (*go_func)(GameObject&);

class TimedTrigger : public GameObject
{
    Timer tm;
    go_func fnc;
public:
    TimedTrigger(GameObject&, float, go_func);
    void Update(float);
};