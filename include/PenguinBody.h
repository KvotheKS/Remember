#pragma once
#include "Component.h"
#include "GameObject.h"
#include "PenguinCannon.h"
#include "InputManager.h"
#include "Camera.h"
#include "Sprite.h"
#include "Bullet.h"
#include "Sound.h"
#include "Collider.h"
#include "Vec2.h"
#include <string>
#include <memory>
#include <algorithm>

#define MAX_SPEED 400
#define MIN_SPEED -250
#define THRESHOLD 5

class PenguinBody : public Component {
    private:
        std::weak_ptr<GameObject> pcannon;
        Vec2 speed;
        float linearSpeed;
        float angle;
        int hp;
    public:
        static PenguinBody* player;
        PenguinBody(GameObject& associated);
        ~PenguinBody();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);
        Vec2 GetPlayerCenter();
};