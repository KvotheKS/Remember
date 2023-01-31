#include "TimeBomb.h"

TimeBomb::TimeBomb(GameObject& associated, float time)
    : GameObject(associated), tm(time)
{}

void TimeBomb::Update(float dt)
{
    if(tm.Update(dt))
    {
        // std::cout << "DIEEEEEEEE" << std::endl;
        associated.RequestDelete();
    }
    // std::cout << tm.Get() << ' ' << tm.GetFinish() << '\n';
}

TimedTrigger::TimedTrigger(GameObject& associated, float time, go_func fnc)
    : GameObject(associated), tm(time), fnc(fnc)
{}

void TimedTrigger::Update(float dt)
{
    if(tm.Update(dt))
    {
        fnc(associated);
        associated.RequestDelete();
    }
}