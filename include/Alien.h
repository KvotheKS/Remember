#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Minion.h"
#include "Camera.h"
#include "Collider.h"
#include "Bullet.h"
#include "PenguinBody.h"
#include "Sound.h"
#include "Timer.h"
#include "Vec2.h"
#include <vector>
#include <limits>
#include <memory>
#include <string>

class Alien : public GameObject {
    private:
        Vec2 speed;
        int hp;
        std::vector<std::weak_ptr<GameObject>> minionArray;
        enum AlienState {MOVING, RESTING};
        AlienState state;
        Timer restTimer;
        Vec2 destination;
        float timeOffset;
    public:
        static int alienCount;
        Alien(GameObject& associated, int nMinions, float timeOffset = 0);
        ~Alien();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);
};