#include "PingusState.h"
#include "GameBoxT.h"

PingusState::PingusState() : State()
{}

PingusState::~PingusState()
{}

void PingusState::LoadAssets()
{
    GameObject* t1 = new GameObject(); // essencialmente o mesmo q a gnt tem. (se vc acessar o associated dele o seu computador explode)
    GameBoxT* t2 = new GameBoxT(*t1, 255, 0, 0); t2->relative = Rect(30, 0, 50, 50); t2->depth = 0;
    GameBoxT* t3 = new GameBoxT(*t2, 0, 255, 0); t3->relative = Rect(0, 50, 100, 20); t3->depth = 1;
    
    t1->AddComponents({t2});
    t2->AddComponent(t3);
    AddObject(t1);
}

void PingusState::Start()
{
    LoadAssets();
    StartArray();
}

void PingusState::Update(float dt)
{
    InputManager& inManager = InputManager::GetInstance();

    if(inManager.QuitRequested())
        quitRequested = true;
    if(inManager.KeyPress(ESCAPE_KEY))
        popRequested = true;
    if(inManager.KeyPress(F3_KEY))
        Collider::debugMode = !Collider::debugMode;

    Camera::Update(dt);

    UpdateArray(dt);
    KillDeads();
}

void PingusState::Render()
{
    RenderArray();
}

void PingusState::Pause()
{}

void PingusState::Resume()
{}