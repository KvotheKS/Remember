#pragma once
#include "GameObject.h"
#include <vector>
#include <memory>
#include <functional>

class State {
    protected:
        bool popRequested;
        bool quitRequested;
        bool started;
        
        virtual void StartArray();
        void StartVector(std::vector<std::shared_ptr<GameObject>>&);
        virtual void UpdateArray(float dt);
        void UpdateVector(std::vector<std::shared_ptr<GameObject>>&, float dt);
        virtual void RenderArray();
        void RenderVector(std::vector<std::shared_ptr<GameObject>>&);
        virtual void KillDeads();
        void KillVector(std::vector<std::shared_ptr<GameObject>>&);
    public:
        std::vector<std::vector<std::shared_ptr<GameObject>>*> arrays;
        std::vector<std::shared_ptr<GameObject>> objectArray; // objetos variados (normalmente sem colisao)
        std::vector<std::shared_ptr<GameObject>> terrainArray; // terreno 
        std::vector<std::shared_ptr<GameObject>> rigidArray; // player/boss (para todas as colisoes)
        std::vector<std::shared_ptr<GameObject>> bulletArray; // bullets (para colisoes de player/terreno)
        
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
        /*
            GetArrays eh um vector de ponteiros para o multivector dentro do state
            todos as funcoes *Array dependem dela, entao se em algum momento for criado
            um vector novo NAO se esquecer de dar um overload nela.
            
            AddObject agora tem um parametro idx, que determina em qual dos vectors o elemento
            deve ser inserido. ELA TAMBEM DEPENDE DO GETARRAYS!!!

            GetObject tem dois formatos diferentes. No mais simples, vc passa um C_ID e o primeiro
            GameObject com um component daquele C_ID eh retornado. No mais complexo, voce deve
            passar uma labmda que recebe cada GameObject guardado nos vectors da state como parametro
            e faz "calculo" para determinar se o GameObject atual eh o que voce procura. 
            EXEMPLOS DA GetObject:
                Se voce quer pegar o player na stack:
                MODO SIMPLES (retorna um GameObject* para o GameObject com um Player dentro):
                    Game::GetInstance()
                    .GetCurrentState()
                    .GetObject(C_ID::Player)
                    .lock()
                    .get();
                MODO "COMPLEXO":
                    return Game::GetInstance()
                    .GetCurrentState()
                    .GetObject([](GameObject& obj) ->bool{return obj.GetComponent(C_ID::Player)})
                    .lock()
                    .get();
            
            Considerando os exemplos acima, pq entao utilizar o modo complexo? Se vc estiver procurando
            um GameObject que nao esta nesse formato simples de GO->Components e sim no formato de Nos,
            vc precisa fazer a lambda pra checar algum no em especifico do GO.
        */
        virtual std::vector<std::vector<std::shared_ptr<GameObject>>*> GetArrays();
        virtual std::weak_ptr<GameObject> AddObject(GameObject* object, int idx = 0);
        virtual std::weak_ptr<GameObject> GetObjectPtr(GameObject* object);
        virtual std::weak_ptr<GameObject> GetObject(std::function<bool(GameObject&)>);
        virtual std::weak_ptr<GameObject> GetObject(C_ID);
        bool PopRequested();
        bool QuitRequested();
      
        std::vector<std::shared_ptr<GameObject>> cameraFollowerObjectArray; //!! scheduler de update will fix this T_T
};