#include <algorithm>
#include <functional>
#include "Component.h"
#include "GameObject.h"
#include <limits>

/*
    How to use: 
    1- This Slider is currently printing according to the GameObject box, so set it 
    accordingly;
    2- rcv is the GameObject which will be passed to "fnc" on every change;
    3- fnc has 2 arguments, the GameObject stored in the slider and pct, which
    is the percentage of the slider (from 0 to 1);
    4- if you want to change how its printed, just make a derived class and
    overload its print function;
*/

class Slider : public Component
{
public:
    float pct;
    GameObject* rcv;
private:
    std::function<void(GameObject*,float)> fnc;

public:
    Slider(GameObject& associated, GameObject* rcv, 
        std::function<void(GameObject*,float)> fnc, 
        float initial = std::numeric_limits<float>::infinity());

public:
    void Update(float dt);
    virtual void Print(float x, float y);
    void Render();
    bool Is(std::string type);
};