#include "ActionMachine.h"

ActionInfo::ActionInfo(std::set<int> actions, float actionTime)
    : actions(actions), actionTime(actionTime)
{}

ActionMachine::ActionMachine(GameObject& Associated)
    : GameObject(Associated)
{ __curr = 0; current_changed = false; just_finished = false; }

void ActionMachine::Update(float dt)
{
    auto it = states.find(__curr);
    if(it == states.end()) return;
    
    just_finished = false;
    auto& info = it->second;
    if(info.timeElapsed > info.actionTime)
    {
        just_finished = true;
        Reset();
        if(info.nxtState != -1)
            __curr = info.nxtState;
    }
    if(!current_changed)
    {
        info.timeElapsed += dt;
        info.count++;
    }
    current_changed = false;
}

void ActionMachine::Render()
{ current_changed = true; }

void ActionMachine::AddState(std::pair<int,ActionInfo> singleState)
{ states.insert(singleState); }

void ActionMachine::AddStates(std::vector<std::pair<int,ActionInfo>> multipleStates)
{
    for(auto& it : multipleStates)
        states.insert(it);
}

std::pair<const int, ActionInfo&> ActionMachine::GetCurrent()
{
    return {__curr, states[__curr]};
}

void ActionMachine::Reset()
{
    auto& info = states[__curr];
    info.timeElapsed = 0.0f;
    info.count = 0;
}

bool ActionMachine::CanChangeTo(int st)
{
    auto& it = states[__curr];
    return it.actions.find(st) != it.actions.end(); 
}

void ActionMachine::ChangeState(int st)
{
    Reset();
    __curr = st;
}

bool ActionMachine::Is(std::string type)
{
    return type == "ActionMachine";
}