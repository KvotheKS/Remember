#include "TimeBomb.h"

TimeBomb::TimeBomb(GameObject& associated, float time)
    : GameObject(associated), tm(time)
{}

void TimeBomb::Update(float dt)
{
    if(tm.Update(dt))
    {
        if(&associated == nullptr)
            RequestDelete();
        else if(&associated.associated == nullptr)
            associated.RequestDelete();
        else
            associated.associated.RemoveComponent(&associated);
    }
}
