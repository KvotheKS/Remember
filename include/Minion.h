#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Bullet.h"
#include "Collider.h"
#include "Vec2.h"
#include <memory>
#include <string>
#include <cstdlib>

class Minion : public GameObject {
    private:
        std::weak_ptr<GameObject> alienCenter;
        float arc;
    public:
        Minion(GameObject& associated, std::weak_ptr<GameObject> alienCenter, float arcOffsetDeg = 0);
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void Shoot(Vec2 target);
};