#include "Scheduler.h"
#include "Game.h"
#include <vector>
#include <algorithm>

std::vector<info> Scheduler::textureVec;

bool SchedulerComp(info a, info b)
{ return a.depth < b.depth; }

void Scheduler::Render()
{
    std::stable_sort(Scheduler::textureVec.begin(), Scheduler::textureVec.end(), SchedulerComp);
    for(auto& it : Scheduler::textureVec)
        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), it.texture, &it.clip, &it.dst,
            it.angle, nullptr, SDL_FLIP_NONE);
    Scheduler::Clear();
}

void Scheduler::Push(SDL_Texture* texture, SDL_Rect& clip, SDL_Rect& dst, float angle, float depth)
{ Scheduler::textureVec.push_back({texture, clip, dst, angle, depth}); }

void Scheduler::Clear()
{ Scheduler::textureVec.clear(); }