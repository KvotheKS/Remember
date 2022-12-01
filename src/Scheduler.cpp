#include "Scheduler.h"
#include "Game.h"
#include <vector>
#include <algorithm>

std::vector<__info> Scheduler::textureVec;

bool SchedulerComp(__info a, __info b)
{ return a.depth < b.depth; }

void Scheduler::Render()
{
    std::stable_sort(Scheduler::textureVec.begin(), Scheduler::textureVec.end(), SchedulerComp);
    for(auto& it : Scheduler::textureVec)
        it.cpt->Print(it.x, it.y);
    
    Scheduler::Clear();
}

void Scheduler::Push(GameObject* cpt, float depth, float x, float y)
{ Scheduler::textureVec.push_back({cpt, depth, x, y}); }

void Scheduler::Clear()
{ Scheduler::textureVec.clear(); }