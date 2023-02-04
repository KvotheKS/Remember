#include "TimeBomb.h"

TimeBomb::TimeBomb(GameObject& associated, float time)
    : GameObject(associated), tm(time)
{}

void TimeBomb::Update(float dt)
{
    if(tm.Update(dt))
    {
        associated.RequestDelete();
    }
}
