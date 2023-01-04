#include "ActionMachine.h"

ActionMachine::ActionMachine(GameObject& Associated)
    : GameObject(Associated)
{ __curr = 0; }

void ActionMachine::Update(float dt)
{
    auto it = states.find(__curr);
    if(it == states.end()) return;
    
    auto& info = it->second;
    if(info.timeElapsed > info.actionTime)
    {
        Reset();
        if(info.noAction != -1)
            __curr = info.noAction;
    }

    info.timeElapsed += dt;
    info.count++;
}

void AddState(std::pair<int,ActionInfo> singleState)
{ states.insert(singleState); }

void AddStates(std::vector<std::pair<int,ActionInfo>> multipleStates)
{
    for(auto& it : multipleStates)
        states.insert(it);
}

std::pair<int,ActionInfo>& GetCurrent()
{
    auto it = states.find(__curr);
    if(it != states.end()) return *it;
    return {__curr, {}};
}

void ActionMachine::Reset()
{
    auto& info = states[__curr];
    info.timeElapsed = 0.0f;
    info.count = 0;
}

void ActionMachine::ChangeState(int st)
{
    Reset();
    __curr = st;
}
