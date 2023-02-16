#pragma once
#include "GameObject.h"
#include "StateMac.h"
#include "Timer.h"
#include "Bcurve.h"
#include "Rand.h"
#include "Spike.h"
#include "FlameSpike.h"
#include <memory>

class MaskBoss : public GameObject {
    private:
        enum {GREEN, RED, YELLOW, DARK = 3};
        enum {PHASE1 = 0, PHASE2 = 6};
        enum {INACTIVED, MOVING, IDLE, CHARGING, ATTAKING, SWAPPING};

        int MAX_HEALTH;
        int currentHealth;

        float MOVE_TIME;
        int CURVE_IDX, MOVE_LOOP;
        vector<Bcurve*> curves;

        float REST_TIME;

        float SWAP_TIME;
        bool SWAPED;

        int CURR_MASK, CURR_PHASE, CURR_STATE;
        Timer timer;
        void _CreateStateMac();
        void Activate();
        void Moving(float dt);
        void Resting(float dt);
        void Swapping(float dt);
        void Attacking();
    public:
        MaskBoss(GameObject&);
        void Update(float);
        bool Is(C_ID);
        void TakeDamage(int damage);
};