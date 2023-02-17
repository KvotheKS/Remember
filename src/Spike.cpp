#include "Spike.h"

void Spike::CreateBigSpike(){
    auto stmac = new StateMachine(associated);
    associated.AddComponents({stmac});

    auto anm = new AnimNode("assets/img/Mask_Atks/bigspike_warning.png", 3, 0.15, Vec2(1, 1));
    stmac->AddNode(WARNING, anm);
    stmac->ChangeState(WARNING);

    anm = new AnimNode("assets/img/Mask_Atks/bigspike_warning_loop.png", 2, 0.15, Vec2(1, 1));
    stmac->AddNode(WARNING_LOOP, anm);
    stmac->AddTransition(WARNING, WARNING_LOOP);

    anm = new AnimNode("assets/img/Mask_Atks/bigspike_active.png", 4, 0.1, Vec2(1, 1));
    stmac->AddNode(ACTIVE, anm);
    stmac->AddTransition(ACTIVE, ACTIVE_LOOP);

    anm = new AnimNode("assets/img/Mask_Atks/bigspike_active_loop.png", 1, 1, Vec2(1, 1));
    stmac->AddNode(ACTIVE_LOOP, anm);

    timer.SetFinish(WARNING_TIME);
    timer.Restart();
}

void Spike::CreateSmallSpike(){
    auto stmac = new StateMachine(associated);
    associated.AddComponents({stmac});

    auto anm = new AnimNode("assets/img/Mask_Atks/smallspike_warning.png", 3, 0.15, Vec2(1, 1));
    stmac->AddNode(WARNING, anm);
    stmac->ChangeState(WARNING);

    anm = new AnimNode("assets/img/Mask_Atks/smallspike_warning_loop.png", 2, 0.15, Vec2(1, 1));
    stmac->AddNode(WARNING_LOOP, anm);
    stmac->AddTransition(WARNING, WARNING_LOOP);

    anm = new AnimNode("assets/img/Mask_Atks/smallspike_active.png", 4, 0.1, Vec2(1, 1));
    stmac->AddNode(ACTIVE, anm);
    stmac->AddTransition(ACTIVE, ACTIVE_LOOP);

    anm = new AnimNode("assets/img/Mask_Atks/smallspike_active_loop.png", 1, 1, Vec2(1, 1));
    stmac->AddNode(ACTIVE_LOOP, anm);

    timer.SetFinish(WARNING_TIME);
    timer.Restart();
}


Spike::Spike(GameObject& associated, int spikeSize, float warningTime, float activeTime, float floorHeight)
    : GameObject(associated)
{
    this->floorHeight = floorHeight;
    WARNING_TIME = warningTime;
    ACTIVE_TIME = activeTime;

    SPIKE_SIZE = spikeSize;
    if(spikeSize == BIG)
        CreateBigSpike();
    else
        CreateSmallSpike();
}

void Spike::Update(float dt){
    auto stmac = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
    if(stmac->GetCurr() == WARNING_LOOP){
        if(timer.Update(dt)){
            timer.Restart();
            timer.SetFinish(ACTIVE_TIME);

            stmac->ChangeState(ACTIVE);
            associated.depth = -associated.depth;
        }
    }
    else if(stmac->GetCurr() == ACTIVE_LOOP){
        if(timer.Update(dt)){
            associated.box.y += associated.box.h/2 * dt;
            if(associated.box.y > floorHeight)
                associated.RequestDelete();
        }
    }
}

bool Spike::Is(C_ID type){
    return type == C_ID::Spike;
}