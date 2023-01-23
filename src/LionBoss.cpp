#include "LionBoss.h"
#include "IA.h"
#include "StateMac.h"
#include "Game.h"
#include <memory>

LionBoss::LionBoss(GameObject& associated)
    : GameObject(associated)
{
    auto lionbrainz = new IA(associated, nullptr, 1.5f);
    auto stmac = new StateMachine(associated);
    associated.AddComponent(lionbrainz);
    associated.AddComponent(stmac);

    auto& currstate = Game::GetInstance().GetCurrentState();
    
    auto target = currstate.GetObject(C_ID::Player);
    if(target.expired())
    {
        std::cout << "PLAYER DEVE SER DECLARADO ATES DO LIONBOSS\n";
        exit(0);
    }

    lionbrainz->SetTarget(target.lock().get());
    lionbrainz->SetActions(
        {
            {Vec2(MIDDLEX, 0), 1, 2.5f},
            {Vec2(MIDDLEX, 0), 1, 2.5f},
            {Vec2(CLOSEX, 0), 1, 3}
        }
    );

}

void LionBoss::Update(float dt)
{
    auto lionbrainz = (IA*)associated.GetComponent(C_ID::IA);
    auto stmac = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    int choice = lionbrainz->selectedAction;
    
    if(choice == -1) return;

    
}