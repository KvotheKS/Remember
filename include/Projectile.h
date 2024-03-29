#pragma once
#include "GameObject.h"
#include "Collider.h"
#include "Timer.h"
#include "Vec2.h"
#include <string>

class Projectile : public GameObject {
    public:
        Vec2 velocity, gravity;
        float pace;
        float homingAccMax, homingRadius, maxSpeed;
        Timer lifeTimeCount;
        bool prftHmg, accHmg, rotSprt;
    public:
        GameObject* target;
        Projectile(GameObject& associated, float lifeTime = std::numeric_limits<float>::infinity(), GameObject* target = nullptr, bool rotSprt = false,
                    float angle = 0.0f, float initialSpeed = 0.0f, float maxSpeed = std::numeric_limits<float>::infinity(), float gravity = 0.0f, float homingRadius = 0.0f,
                    float homingAccMax = 0.0f, bool prftHmg = false, bool accHmg = false, float pace = 1.0f);
        //Projetil simples
        Projectile(GameObject& associated, float lifeTime, float angle = 0.0f, float initialSpeed = 0.0f,
                float maxSpeed = std::numeric_limits<float>::infinity(), float gravity = 0.0f, float pace = 1.0f);
        //Projetil com homing
        Projectile(GameObject& associated, float lifeTime, GameObject* target, float angle = 0.0f, float initialSpeed = 0.0f, 
                float maxSpeed = std::numeric_limits<float>::infinity(), float homingRadius = 0.0f,
                    float homingAccMax = 0.0f, bool accelerated = false, float gravity = 0.0f, float pace = 1.0f);
        void Update(float dt);
        bool Is(C_ID);
        void SetMaxSpeed(float maxSpeed);
};