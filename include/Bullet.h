#pragma once
#include "GameObject.h"
#include "Component.h"
#include "Sprite.h"
#include "Collider.h"
#include "Vec2.h"
#include <string>

class Bullet : public GameObject {
    private:
        Vec2 speed;
        float distanceLeft;
        int damage;
    public:
        bool targetsPlayer;
        Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance,
               std::string sprite, int frameCount, float frameTime, bool targetsPlayer);
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        bool Is(C_ID);
        int GetDamage();
};