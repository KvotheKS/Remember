#pragma once
// #include "Component.h"
// #include "TerrainBody.h"
// #include "RigidBody.h"
#include "ObjectId.h"
#include "Rect.h"
#include <vector>
#include <memory>
#include <string>


class GameObject {
    private:
        std::vector<std::unique_ptr<GameObject>> components;
    protected:
        GameObject& associated;
        bool isDead;
    public:
        Rect box, relative;
        bool started;
        double angleDeg;
        float depth;
    public:
        GameObject(GameObject& associated = *((GameObject*)nullptr));
        ~GameObject();
        virtual void Start();
        void UpdateNodes(float dt);
        void RenderNodes();
        virtual void Update(float dt);
        virtual void Render();
        virtual void Print(float x = -1, float y = -1);
        virtual bool Is(std::string type);
        virtual bool Is(C_ID);
        virtual std::string Is();
       
        bool IsDead();
        void RequestDelete();

        void AddComponent(GameObject* cpt);
        void AddComponents(std::initializer_list<GameObject*> cpts);
        void RemoveComponent(GameObject* cpt);
        
        GameObject* GetComponent(std::string type);
        GameObject* GetComponent(C_ID type);
        

        std::vector<GameObject*> GetComponents(std::string type);
        std::vector<GameObject*> GetComponents(C_ID type);

        virtual void NotifyCollision(GameObject& other,Vec2 sep);
        void NotifyCollisionBehavior(GameObject& other,Vec2 sep);

};