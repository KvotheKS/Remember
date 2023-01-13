#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Camera.h"
#include "Vec2.h"
#include "Rect.h"
#include <string>

class Collider : public GameObject {
    private:
        Vec2 scale;
        Vec2 offset;
    public:
        // C_ID local_type;
        static bool debugMode;
        Rect box;
        Collider(GameObject& associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0});
        void Start();
        void Update(float dt);
        void Render();
        void Print(float x=-1, float y=-1);
        bool Is(std::string type);
        bool Is(C_ID);
        void SetScale(Vec2 scale);
        void SetOffset(Vec2 offset);
        Vec2 GetScale();
        Vec2 GetOffset();
};