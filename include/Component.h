#pragma once
#include <string>
#include "Vec2.h"
class GameObject;

class Component{
    protected:
        GameObject& associated;
        // Rect absolute, relative;
        // std::vector<std::unique_ptr<Component>> components;
        bool isDead;
    public:
        Component(GameObject& associated);
        virtual ~Component();
        virtual void Start();
        virtual void Update(float dt) = 0;
        virtual void Render() = 0;
        virtual void Print(float x = -1, float y = -1);
        virtual bool Is(std::string type) = 0;
        virtual void NotifyCollision(GameObject& other,Vec2 sep);
};