#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Camera.h"
#include "Vec2.h"
#include "Rect.h"
#include <string>

class Collider : public Component {
    private:
        Vec2 scale;
        Vec2 offset;
    public:
        static bool debugMode;
        Rect box;
        Collider(GameObject& associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0});
        void Update(float dt);
        void Render();
        void Print(float x=-1, float y=-1);
        bool Is(std::string type);
        void SetScale(Vec2 scale);
        void SetOffset(Vec2 offset);
};