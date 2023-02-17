#include "ProjectileB.h"

ProjectileB::ProjectileB(GameObject& associated, Bcurve* curve, float lifeTime, float maxMoveTime, bool rotSprt)
    : GameObject(associated)
{
    trajectory = curve;
    if(rotSprt){
        velocity = curve->GetDerivate();
        Vec2 direction = velocity->GetNewPoint(0.0f);
        associated.angleDeg = direction.AngleX() * PI_DEG;
    }

    lifeTimeCount.Restart();
    lifeTimeCount.SetFinish(lifeTime);

    MOVEDURATION = maxMoveTime;
    movingTimer.Restart();
    movingTimer.SetFinish(maxMoveTime);

    this->rotSprt = rotSprt;
}

void ProjectileB::Update(float dt){
    Vec2 pos, dir;
    if(lifeTimeCount.Update(dt))
        associated.RequestDelete();
    else{
        if(movingTimer.Update(dt)){
            pos = trajectory->GetNewPoint(1.0f);
            if(rotSprt)
                dir = velocity->GetNewPoint(1.0f);
        }
        else{
            float t = movingTimer.Get() / MOVEDURATION;
            pos = trajectory->GetNewPoint(t);
            if(rotSprt)
                dir = velocity->GetNewPoint(t);
        }
        associated.box.SetCenter(pos.x, pos.y);
        if(rotSprt)
            associated.angleDeg = dir.AngleX() * PI_DEG;
    }
}

bool ProjectileB::Is(C_ID type){
    return type == C_ID::ProjectileB;
}