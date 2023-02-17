#include "FlameSpike.h"

FlameSpike::FlameSpike(GameObject& associated, std::weak_ptr<GameObject> center, float lifeTime)
    : GameObject(associated)
{
    auto stmac = new StateMachine(associated);
    auto collider = new Collider(associated);
    collider->SetScale(Vec2(0.8, 0.8));
    associated.AddComponents({stmac, collider});
    auto anm = new AnimNode("assets/img/Mask_Atks/flameSpikes.png", 10, 0.1);
    anm->SetScaleX(1.65, 1.65);
    stmac->AddNode(0, anm);
    stmac->ChangeState(0);

    timer.SetFinish(lifeTime);
    timer.Restart();

    this->center = center;
    if(std::shared_ptr<GameObject> orig = this->center.lock()){
        Vec2 maskCenter = orig->box.GetCenter();
        associated.box.SetCenter(maskCenter.x, maskCenter.y);
    }
}

void FlameSpike::Update(float dt){
    if(std::shared_ptr<GameObject> orig = this->center.lock()){
        Vec2 maskCenter = orig->box.GetCenter();
        associated.box.SetCenter(maskCenter.x, maskCenter.y);
        if(timer.Update(dt))
            associated.RequestDelete();
    }
    else
        associated.RequestDelete();
}

bool FlameSpike::Is(C_ID type){
    return type == C_ID::FlameSpike;
}