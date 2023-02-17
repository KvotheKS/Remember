#include "Laser.h"
#include "Game.h"

Laser::Laser(GameObject& associated, float laseringTime) : GameObject(associated){
    LASERING_TIME = laseringTime;
    MARKED = false;
    angle = 0.0;

    auto stmac = new StateMachine(associated);
    associated.AddComponents({stmac});

    auto anm = new AnimNode("assets/img/Mask_Atks/laser_charge.png", 7, 0.05, Vec2(0.25, 0.25));
    stmac->AddNode(CHARGING, anm);
    stmac->ChangeState(CHARGING);

    anm = new AnimNode("assets/img/Mask_Atks/laser_fire.png", 4, 0.05, Vec2(1, 0.25));
    stmac->AddNode(FIRING, anm);
    stmac->AddTransition(CHARGING, FIRING);

    anm = new AnimNode("assets/img/Mask_Atks/laser_loop.png", 1, 1, Vec2(1, 0.25));
    stmac->AddNode(LASER_LOOP, anm);
    stmac->AddTransition(FIRING, LASER_LOOP);

    timer.SetFinish(LASERING_TIME);
    timer.Restart();
}

void Laser::Update(float dt){
    auto stmac = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
    if(stmac->GetCurr() == CHARGING)
        center = associated.box.GetCenter();
    else if(stmac->GetCurr() == FIRING && !MARKED){
        auto& st = Game::GetInstance().GetCurrentState();
        auto target = st.GetObject(C_ID::Player, &st.rigidArray);

        float angle = center.AngleLine(target.lock().get()->box.GetCenter());
        Vec2 rotCenter = center + Vec2(associated.box.w/2, 0).Rotate(angle);
        associated.box.SetCenter(rotCenter.x, rotCenter.y);
        associated.angleDeg = angle * PI_DEG;

        MARKED = true;
    }
    else if(stmac->GetCurr() == LASER_LOOP){
        if(MARKED){
            auto collider = new Collider(associated);
            associated.AddComponent(collider);
            MARKED = false;
        }
        if(timer.Update(dt))
            associated.RequestDelete();
    }
}

void Laser::Render(){
}

bool Laser::Is(C_ID type){
    return type == C_ID::Laser;
}