#include "IA.h"
#include "Rand.h"
#include <limits>

IA::IA(GameObject& associated, GameObject* target, float positionWeight)
    : GameObject(associated), target(target), positionWeight(positionWeight)
{ rendered = false; times = 0;}

void IA::Update(float dt)
{
    if(!rendered || !tm.Update(dt) || !target)
    { 
        selectedAction = -1;
        return;
    }

    float overallWeight = 0.0f;
    
    for(auto& it : actions)
    {
        Heuristic(it);
        overallWeight += it.weight;
    }

    double pct = ((double)Rand::Get()) / ((double)std::numeric_limits<int>::max());
    float buildingWeight = 0.0f;
    for(unsigned int i = 0; i < actions.size(); i++)
    {
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
    float dst = (box.GetCenter() + it.range - target->box.GetCenter()).Magnitude();
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
