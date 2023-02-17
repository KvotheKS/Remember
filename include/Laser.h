#pragma once
#include "GameObject.h"
#include "StateMac.h"
#include "Timer.h"

class Laser : public GameObject {
    public:
        enum {CHARGING, FIRING, LASER_LOOP};
        float angle;
        float LASERING_TIME;
        bool MARKED;
        Timer timer;
        Vec2 center;
    public:
        Laser(GameObject& associated, float laseringTime);
        void Update(float dt);
        void Render();
        bool Is(C_ID type);
};