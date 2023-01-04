#pragma once

#include "GameObject.h"
#include <unordered_map>

struct ActionInfo
{
    std::set<int> actions;
    int count = 0;
    float timeElapsed = 0.0f, actionTime = 0.0f;
    int noAction = -1;
};

class ActionMachine : public GameObject
{
    int __curr;
    std::unordered_map<int, ActionInfo> states;
public:
    ActionMachine(GameObject&);
    void Update(float dt);
    void AddState(std::pair<int,ActionInfo>);
    void AddStates(std::vector<std::pair<int,ActionInfo>>);
    void Reset();
    void ChangeState(int);
    std::pair<int,ActionInfo>& GetCurrent();
};