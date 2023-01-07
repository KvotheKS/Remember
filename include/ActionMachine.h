#pragma once

#include "GameObject.h"
#include <set>
#include <unordered_map>

struct ActionInfo
{
    std::set<int> actions;
    int count = 0;
    float timeElapsed = 0.0f, actionTime = 0.0f;
    int nxtState = -1;
    ActionInfo(std::set<int>, float);
    ActionInfo() = default;
};

class ActionMachine : public GameObject
{
    int __curr;
    bool current_changed;
    bool just_finished;
    std::unordered_map<int, ActionInfo> states;
public:
    ActionMachine(GameObject&);
    void Update(float dt);
    void Render();
    void AddState(std::pair<int,ActionInfo>);
    void AddStates(std::vector<std::pair<int,ActionInfo>>);
    void Reset();
    bool CanChangeTo(int);
    void ChangeState(int);
    std::pair<const int,ActionInfo&> GetCurrent();
    bool Is(std::string);
};