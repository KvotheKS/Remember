#pragma once

#include "GameObject.h"
#include <vector>
#include "Rect.h"
#include "Timer.h"
#include <cmath>

struct ActionInfo
{
    Vec2 range;
    float rarity;
    float cooldown;
    float weight;
};

class IA : public GameObject
{
public:
    Timer tm;
    std::vector<ActionInfo> actions;
    GameObject* target;
    float positionWeight;
    int selectedAction;
    bool rendered;
public:
    IA(GameObject&, GameObject*, float);
    void Update(float);
    void Render();
    void SetActionTimer(int);
    void SetActionTimer(float);
    void SetActions(std::vector<ActionInfo>);
    void AddAction(std::vector<ActionInfo>);
    void AddAction(ActionInfo);
    void Heuristic(ActionInfo&);
    void SetTarget(GameObject*);
    bool Cooling();
    bool Is(C_ID);
};