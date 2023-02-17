#pragma once
#include "GameObject.h"
#include "StateMac.h"
#include "Timer.h"
#include "Bcurve.h"
#include "Collider.h"
#include "Sprite.h"
#include "TimeBomb.h"
#include "Rand.h"
#include "Spike.h"
#include "FlameSpike.h"
#include "Projectile.h"
#include "ProjectileB.h"
#include "Laser.h"
#include <memory>

class MaskBoss : public GameObject {
    private:
        enum {GREEN, RED, YELLOW, DARK = 3};
        enum {PHASE1 = 0, PHASE2 = 6};
        enum {INACTIVED, MOVING, IDLE, CHARGING, ATTAKING, COOLING, SWAPPING};

        int MAX_HEALTH;
        int currentHealth;

        float MOVE_TIME, MOVE_TIME2;
        int CURVE_IDX, MOVE_LOOP;
        vector<Bcurve*> curves;

        float REST_TIME;

        float CHARGE_TIME;
        bool CHARGED;

        bool DOUBLE_HIT;

        float SWAP_TIME;

        int SPIKE_NUM;

        float SHOOTING_TIME, SHOOTING_COOLDOWN;
        int SHOOT_NUM;
        bool CHARGE_SHOOT, COOLDOWN_SHOOT;

        float LASERING_TIME, LASERING_COOLDOWN;

        float RAINING_TIME, DELAY_RAIN;

        int CURR_MASK, CURR_PHASE, CURR_STATE, CURR_ATK;
        Timer timer;
        void Activate();
        void Moving(float dt);
        void Resting(float dt);
        void Charging(float dt);
        void Spiking(int spikeSize, int spikeNum);
        void Flaming();
        void Attacking(float dt);
        void Lasering(float dt);
        void Shooting(float dt);
        void Raining();
        void Cooling(float dt);
        void Swapping(float dt);
    public:
        MaskBoss(GameObject&);
        void Update(float);
        bool Is(C_ID);
        void TakeDamage(int damage);
};