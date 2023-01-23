#pragma once
#include "GameObject.h"
#include "Collider.h"
#include "Timer.h"
#include "Vec2.h"
#include <string>

class Projectile : public GameObject {
    private:
        Vec2 velocity, gravity, acceleration;
        float homingAccMax, homingRadius, maxSpeed;
        int damage;
        Timer lifeTimeCount;
        float lifeTime;
        bool prftHmg, accHmg, rotSprt;
    public:
        bool targetsPlayer;
        Projectile(GameObject& associated, int damage, float lifeTime, bool targetsPlayer, bool rotSprt,
                    float angle, float initialSpeed, float maxSpeed, float gravity, float homingRadius,
                    float homingAccMax, bool prftHmg, bool accHmg, Vec2 acceleration = {0,0});
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        bool Is(C_ID);
        int GetDamage();
        void SetMaxSpeed(float maxSpeed);
};