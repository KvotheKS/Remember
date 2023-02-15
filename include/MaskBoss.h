#pragma once
#include "GameObject.h"
#include "StateMac.h"
#include "Timer.h"
#include "Bcurve.h"
#include "Rand.h"

class MaskBoss : public GameObject {
    private:
        enum {GREEN, RED, YELLOW, DARK = 3};
        enum {PHASE1 = 0, PHASE2 = 6};
        enum {INACTIVED, MOVING, IDLE, CHARGING, ATKING, SWAPPING};

        int MAX_HP;
        int hp;

        float MOVETIME;
        int CURVEIDX, MOVELOOP;
        vector<Bcurve*> curves;

        float RESTTIME;

        float SWAPTIME;
        bool SWAPED;

        int CURR_MASK, CURR_PHASE, CURR_STATE;
        Timer timer;
        void _CreateStateMac();
        void Activate();
        void Moving(float dt);
        void Resting(float dt);
        void Swapping(float dt);
    public:
        MaskBoss(GameObject&);
        void Update(float);
        bool Is(C_ID);
        void TakeDamage(int damage);
};