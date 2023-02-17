#include "TimeBomb.h"

TimeBomb::TimeBomb(GameObject& associated, float time)
    : GameObject(associated), tm(time)
{}

void TimeBomb::Update(float dt)
{
    if(rendered && tm.Update(dt))
    {
        if(&associated == nullptr)
            RequestDelete();
        else if(&associated.associated == nullptr)
            associated.RequestDelete();
        else
            associated.associated.RemoveComponent(&associated);
    }
}

void TimeBomb::Render()
{rendered = true;}

bool TimeBomb::Is(C_ID type)
{ return type == C_ID::TimeBomb; }
