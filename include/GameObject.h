#pragma once
#include "Component.h"
#include "Rect.h"
#include <vector>
#include <memory>
#include <string>

class GameObject {
    private:
        std::vector<std::unique_ptr<Component>> components;
        bool isDead;
    public:
        Rect box;
        bool started;
        double angleDeg;
        float depth;
        GameObject();
        ~GameObject();
        void Start();
        void Update(float dt);
        void Render();
        bool IsDead();
        void RequestDelete();
        void AddComponent(Component* cpt);
        void RemoveComponent(Component* cpt);
        Component* GetComponent(std::string type);
        void NotifyCollision(GameObject& other);
};