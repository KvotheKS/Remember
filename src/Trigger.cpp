#include "Trigger.h"

TimedTrigger::TimedTrigger(GameObject& associated, float time, go_func fnc, int repeating)
    : GameObject(associated), tm(time), fnc(fnc), repeating(repeating)
{}

void TimedTrigger::Update(float dt)
{
    if(rendered && tm.Update(dt))
    {
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

void TimedTrigger::Render()
{ rendered = true; }

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

AltenatingTrigger::AltenatingTrigger(GameObject& associated, float frequency, float duration, go_func frst, go_func snd)
    : GameObject(associated), duration(duration), frequency(frequency)
{
    this->frst = frst;
    this->snd = snd;
    __curr = 0;
}

void AltenatingTrigger::Update(float dt)
{
    if(rendered && frequency.Update(dt))
    {
        frequency.Restart();
        if(__curr) snd(associated);
        else frst(associated);
        __curr = !__curr;
    }
    if(rendered && duration.Update(dt))
        associated.RemoveComponent(this);
}

void AltenatingTrigger::Render()
{rendered =true;}

ProgressTrigger::ProgressTrigger(GameObject& associated, float duration, prog_func progress)
    : GameObject(associated), duration(duration)
{
    this->progress = progress;
}

void ProgressTrigger::Update(float dt)
{
    if(rendered && duration.Update(dt))
    {
        progress(associated, 1.0f);
        associated.RemoveComponent(this);
        return;
    }
    progress(associated, duration.Get()/duration.GetFinish());
}

void ProgressTrigger::Render()
{ rendered = true;}