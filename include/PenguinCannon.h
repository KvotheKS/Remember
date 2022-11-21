#pragma once
#include "Component.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Camera.h"
#include "Sprite.h"
#include "Bullet.h"
#include "Collider.h"
#include "Timer.h"
#include "Vec2.h"
#include <string>
#include <memory>

class PenguinCannon : public Component {
    private:
        std::weak_ptr<GameObject> pbody;
        float angle;
        Timer timer;
    public:
        PenguinCannon(GameObject& associated, std::weak_ptr<GameObject> penguinBody);
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void Shoot();
};