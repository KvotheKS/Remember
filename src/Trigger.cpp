#include "Trigger.h"

TimedTrigger::TimedTrigger(GameObject& associated, float time, go_func fnc, int repeating)
    : GameObject(associated), tm(time), fnc(fnc), repeating(repeating)
{}

void TimedTrigger::Update(float dt)
{
    if(tm.Update(dt))
    {
        // std::cout << "CHGUSAJJKSR\n";
        fnc(associated);
        if(repeating == 0)
        {
            associated.RemoveComponent(this);
            return;
        }
        repeating -= (repeating > 0);
        tm.Restart();
    }
}

ChainTrigger::ChainTrigger(GameObject& associated, std::vector<TriggerNode> tNodes, int repeating)
    : GameObject(associated), tNodes(tNodes), repeating(repeating)
{
    __curr = 0;
    tm.Restart();
    tm.SetFinish(tNodes[__curr].timer);
}

void ChainTrigger::Update(float dt)
{
    if(tm.Update(dt))
    {
        tNodes[__curr].fnc(associated);
        __curr++;
        if(__curr == tNodes.size())
        {
            if(repeating == 0)
            {
                associated.RemoveComponent(this);
                return;
            }
            __curr = 0;
        }
        repeating -= (repeating>0);
        tm.SetFinish(tNodes[__curr].timer);
        tm.Restart();
    }
}

ConditionalTrigger::ConditionalTrigger(GameObject& associated, condgo_func cond, go_func trig, int repeating)
    : GameObject(associated), cond(cond), trig(trig), repeating(repeating)
{}

void ConditionalTrigger::Update(float dt)
{
    if(cond(associated))
    {
        trig(associated);
        if(repeating == 0)
            associated.RemoveComponent(this);
        repeating -= (repeating>0);
    }
}