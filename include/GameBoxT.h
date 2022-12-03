#pragma once 

#include "Game.h"
#include "GameObject.h"
#include <string>
#include "Camera.h"
#define INCLUDE_SDL
#include "SDL_include.h"

class GameBoxT : public GameObject
{
    Uint8 r,g,b;
public:
    GameBoxT(GameObject& associated, Uint8 r, Uint8 g, Uint8 b);

public:
    void Render();
    void Print(float x, float y);
    bool Is(std::string type);
    std::string Is();
};