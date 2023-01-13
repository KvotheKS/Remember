#pragma once
#include "GameObject.h"
#include <vector>
#include <memory>

class State {
    protected:
        bool popRequested;
        bool quitRequested;
        bool started;
        std::vector<std::shared_ptr<GameObject>> objectArray; // objetos variados (normalmente sem colisao)
        std::vector<std::shared_ptr<GameObject>> terrainArray; // terreno 
        std::vector<std::shared_ptr<GameObject>> rigidArray; // player/boss (para todas as colisoes)
        std::vector<std::shared_ptr<GameObject>> bulletArray; // bullets (para colisoes de player/terreno)
        
        
        virtual void StartArray();
        void StartVector(std::vector<std::shared_ptr<GameObject>>&);
        virtual void UpdateArray(float dt);
        void UpdateVector(std::vector<std::shared_ptr<GameObject>>&, float dt);
        virtual void RenderArray();
        void RenderVector(std::vector<std::shared_ptr<GameObject>>&);
        virtual void KillDeads();
        void KillVector(std::vector<std::shared_ptr<GameObject>>&);
    
    public:
        State();
        virtual ~State();
        virtual void LoadAssets() = 0;
        virtual void Start() = 0;
        virtual void Update(float dt) = 0;
        
        virtual void Render() = 0;
        virtual void Pause() = 0;
        virtual void Resume() = 0;
        virtual void Collision() = 0;
        virtual std::weak_ptr<GameObject> AddObject(GameObject* object);
        virtual std::weak_ptr<GameObject> GetObjectPtr(GameObject* object);
        bool PopRequested();
        bool QuitRequested();
      
        std::vector<std::shared_ptr<GameObject>> cameraFollowerObjectArray; //!! scheduler de update will fix this T_T
};