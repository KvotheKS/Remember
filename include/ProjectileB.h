#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "Collider.h"
#include "Bcurve.h"
#include "Vec2.h"

class ProjectileB : public GameObject {
    private:
        float MOVEDURATION;
        Timer lifeTimeCount, movingTimer;
        Bcurve* trajectory;
        Bcurve* velocity;
        bool rotSprt = false;
    public:
        ProjectileB(GameObject& associated, Bcurve* curve, float lifeTime, float maxMoveTime, bool rotSprt = false);
        void Update(float dt);
        bool Is(C_ID type);
};