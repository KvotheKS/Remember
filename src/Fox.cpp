#include "Fox.h"
#include "StateMac.h"
#include "IA.h"
#include "Game.h"
#include "Collider.h"

void Fox::SetVariables()
{
    auto& st = Game::GetInstance().GetCurrentState();
    FOXSIZE = Vec2(650, 500);

    ARCBALLSSIZE;
    ARCBALLSQTT;

    KONSIZE;
    KONSETUPTIME;

    BULLETHELLDURATION;

    WÕEXSPEED; // Wõe eh o que cria 2 projeteis q vão dos pontos do estágio e quando os projeteis se encontram eles invertem o angulo 180graus

    LIONPHASINGTIME;
    LIONLASERDURATION;

    COMETSSPACEBETWEEN;
    COMETSIZE;
    COMETSPEED;

    activated = false;
    moving = false;
    phase2 = false;
    currAttack = -1;


    player = st.GetObject(C_ID::Player, &st.rigidArray).lock().get();
}

Fox::Fox(GameObject& associated)
    : GameObject(associated)
{
    SetVariables();

    auto& st = Game::GetInstance().GetCurrentState();
    auto stmac = new StateMachine(associated);
    auto cld = new Collider(associated);
    
    auto node  = new AnimNode("assets/img/Fox/Kihiko Idle1.png", 1,0);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(IDLE, node);

    node = new AnimNode("assets/img/Fox/Kihiko Bullethell K.png", 1, 0);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(BULLETHELLANIM, node); stmac->AddTransition(BULLETHELLANIM, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Dano K.png",1,0);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(DAMAGED, node); stmac->AddTransition(DAMAGED, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Kon K.png", 1,0);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(KONANIM, node); stmac->AddTransition(KONANIM, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Snap K.png", 1, 0);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(SNAPANIM, node); stmac->AddTransition(SNAPANIM, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Stun K.png", 1,0);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(STUNNED, node); stmac->AddTransition(STUNNED, IDLE);
    
    auto brainz = new IA(associated, st.GetObject(C_ID::Player, &st.rigidArray).lock().get(), 1.2f);
    
    
    
    associated.AddComponents({cld, stmac, brainz});
}


void Fox::Update(float dt)
{
    auto st = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    auto [idx, currNode] = st->GetCurrent();

    if(idx != STUNNED)
    {
        currNode->SetFliped((player->box.GetCenter().x < this->box.GetCenter().x));
    }
}

void Fox::ARCBALL_f()
{

}

void Fox::KON_f()
{

}

void Fox::BULLETHELL_f()
{

}

void Fox::WÕE_f()
{

}

void Fox::LIONLASERS_f()
{

}

void Fox::COMET_f()
{

}

void Fox::Phase2Transition()
{
    
}