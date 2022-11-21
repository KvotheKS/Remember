#pragma once

#include "Component.h"
#include "GameObject.h"

class FpsPrinter : public Component
{
    int count;
    float dt;
public:
    FpsPrinter(GameObject& associated);

public:
    void Update(float dt);
    void Render();

public:
    bool Is(std::string type);
};