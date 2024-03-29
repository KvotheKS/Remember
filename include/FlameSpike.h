#pragma once
#include "GameObject.h"
#include "StateMac.h"
#include "Collider.h"
#include "Timer.h"

class FlameSpike : public GameObject {
    std::weak_ptr<GameObject> center;
    Timer timer;
    public:
        FlameSpike(GameObject& associated, std::weak_ptr<GameObject> center, float lifeTime);
        void Update(float dt);
        bool Is(C_ID type);
};