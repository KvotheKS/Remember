#pragma once
#include "GameObject.h"
#include "StateMac.h"
#include "Timer.h"

class Spike : public GameObject {
    private:
        enum {WARNING, WARNING_LOOP, ACTIVE, ACTIVE_LOOP};
        float floorHeight;
        float WARNING_TIME, ACTIVE_TIME;
        int SPIKE_SIZE;
        Timer timer;
        void CreateBigSpike();
        void CreateSmallSpike();
    public:
        enum {SMALL, BIG};
        Spike(GameObject& associated, int spikeSize, float warningTime, float activeTime, float floorHeight);
        void Update(float dt);
        bool Is(C_ID type);
};