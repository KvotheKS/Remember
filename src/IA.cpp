#include "IA.h"
#include "Rand.h"
#include "Game.h"
#include <limits>

IA::IA(GameObject& associated, GameObject* target, float positionWeight)
    : GameObject(associated), target(target), positionWeight(positionWeight), tm(0.1f)
{ rendered = false; }


void IA::Update(float dt)
{
    auto& st = Game::GetInstance().GetCurrentState();

    if(!rendered || !tm.Update(dt) || !target || st.GetObject(C_ID::Player, &st.rigidArray).expired())
    {
        // std::cout<<'\n'<< tm.Get(); 
        selectedAction = -1;
        return;
    }

    float overallWeight = 0.0f;
    
    for(auto& it : actions)
    {
        if(it.deactivated) continue;
        Heuristic(it);
        overallWeight += it.weight;
    }

    double pct = Rand::Get_r();
    float buildingWeight = 0.0f;
    for(unsigned int i = 0; i < actions.size(); i++)
    {
        if(actions[i].deactivated) continue;
        buildingWeight += actions[i].weight / overallWeight;
        if(pct <= buildingWeight)
        {
            selectedAction = i;
            SetActionTimer((int)i);
            return;
        }
    }
}

void IA::Heuristic(ActionInfo& it)
{
    auto tgcntr = target->box.GetCenter();
    float dst = (box.GetCenter() + it.range.Rotate((tgcntr - box.GetCenter()).AngleX()) - tgcntr).Magnitude();
    dst = std::pow(max(dst, 1.0f),positionWeight);
    it.weight = ((1/dst) / it.rarity);
}

void IA::Render()
{ 
    rendered = true; selectedAction = -1; 
}

void IA::SetActionTimer(int idx)
{ SetActionTimer(actions[idx].cooldown); }

void IA::SetActionTimer(float finish)
{
    tm.Restart(); tm.SetFinish(finish); rendered = false;
}

void IA::SetActions(std::vector<ActionInfo> actions)
{ this->actions = actions; }

void IA::AddAction(std::vector<ActionInfo> actions)
{ this->actions.insert(this->actions.end(), actions.begin(), actions.end()); }

void IA::AddAction(ActionInfo action)
{ this->actions.push_back(action); }

void IA::SetTarget(GameObject* target)
{ this->target = target; }

bool IA::Cooling()
{ return !tm.Update(0.0f); }

bool IA::Is(C_ID type)
{ return type == C_ID::IA; }
