#pragma once

#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"

class ShakeBehavior : public GameObject
{
public:
    Vec2 fromOgn;
    Vec2 destination;
    Vec2 dstdx;
    Vec2 amplitude;
    Timer duration, frequency;
    bool comeBack;
public:
    ShakeBehavior(GameObject&, Vec2, float, float);
    void Start();
    void ShakeYoBooty();
    void Update(float);
    bool Is(C_ID);
};