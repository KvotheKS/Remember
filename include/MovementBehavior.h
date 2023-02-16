#pragma once

#include "GameObject.h"
#include "Timer.h"
#include "Vec2.h"

class MovementBehavior : public GameObject{
private:
    int type;
    float speed;
    Timer timer;
    float duration;
    bool fase;
public:
    MovementBehavior(GameObject& associated, float speed = 100, float duration = 1, int type = 0);
    ~MovementBehavior();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    bool Is(C_ID);
};