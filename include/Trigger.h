#pragma once

#include "GameObject.h"
#include "Timer.h"
#include <functional>

typedef void (*go_func)(GameObject&);
typedef bool (*condgo_func)(GameObject&);
typedef void (*prog_func)(GameObject&, float);

class TimedTrigger : public GameObject
{
    Timer tm;
    go_func fnc;
    int repeating;
public:
    TimedTrigger(GameObject&, float, go_func, int = 0);
    void Update(float);
};

struct TriggerNode
{
    float timer;
    go_func fnc;
};

class ChainTrigger : public GameObject
{
    int __curr;
    std::vector<TriggerNode> tNodes;
    Timer tm;
    int repeating;
public:
    ChainTrigger(GameObject&, std::vector<TriggerNode>, int = 0);
    void Update(float);
};

class ConditionalTrigger : public GameObject
{
    condgo_func cond;
    go_func trig;
    int repeating;
public:
    ConditionalTrigger(GameObject&, condgo_func, go_func, int = 0);
    void Update(float);
};

class AltenatingTrigger : public GameObject
{
    go_func frst, snd;
    int __curr;
    Timer duration, frequency;
public:
    AltenatingTrigger(GameObject&, float, float, go_func, go_func);
    void Update(float);
};

class ProgressTrigger : public GameObject
{
    prog_func progress;
    Timer duration;
public:
    ProgressTrigger(GameObject&, float, prog_func);
    void Update(float);
};