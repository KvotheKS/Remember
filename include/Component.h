#pragma once
#include <string>

class GameObject;

class Component {
    protected:
        GameObject& associated;
    public:
        Component(GameObject& associated);
        virtual ~Component();
        virtual void Start();
        virtual void Update(float dt) = 0;
        virtual void Render() = 0;
        virtual void Print(float x = -1, float y = -1);
        virtual bool Is(std::string type) = 0;
        virtual void NotifyCollision(GameObject& other);
};