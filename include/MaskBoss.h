#pragma once
#include "GameObject.h"
#include "StateMac.h"
#include "Timer.h"
#include "Bcurve.h"
#include "Rand.h"

class MaskBoss : public GameObject {
    public:
        enum {PHASE1, PHASE2, PHASE3};
        enum {RED, GREEN, YELLOW};
        enum {IDLE, MOVE, CHARGE, ATK1, ATK2, CHANGE};
        enum {LEFT, CENTER, RIGHT};
        Timer t;
        int idx, flag;

        float MAXMOVETIME;
        vector<Bcurve*> curves;

        MaskBoss(GameObject&);
        void Update(float);
        bool Is(C_ID);
};