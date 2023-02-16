#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "Bcurve.h"
#include "Vec2.h"

class ProjectileB : public GameObject {
    private:
        float MOVEDURATION;
        Timer lifeTimeCount, movingTimer;
        int damage;
        Bcurve* trajectory;
        Bcurve* velocity;
        bool rotSprt = false;
    public:
        ProjectileB(GameObject& associated, Bcurve* curve, float lifeTime,
            float maxMoveTime, int damage, bool rotSprt = false);
        void Update(float dt);
        void Render();
        bool Is(C_ID type);
        int GetDamage();
};