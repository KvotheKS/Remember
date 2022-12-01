#include "Slider.h"
#include "Game.h"
#include "InputManager.h"
#include "Vec2.h"
#include "Scheduler.h"
#define INCLUDE_SDL
#include "SDL_include.h"

Slider::Slider(GameObject& associated, GameObject* rcv, std::function<void(GameObject*, float)> fnc, float initial)
    : Component(associated)
{
    this->rcv = rcv;
    this->pct = (initial == std::numeric_limits<float>::infinity()) ? 0.5 : initial;
    this->fnc = fnc;
    this->fnc(rcv, pct);
}

void Slider::Update(float dt)
{
    associated.box += Camera::pos-Camera::lpos; // CameraFollower.
    InputManager& inp = InputManager::GetInstance();
    Vec2 mpos = Vec2(inp.GetMouseX(), inp.GetMouseY()) + Camera::pos;
    if(associated.box.IsInside(mpos))
    {
        if(inp.IsMouseDown(SDL_BUTTON_LEFT))
        {
            float perct = (mpos.x-associated.box.x)/(associated.box.w);
            this->pct = perct;
            this->fnc(rcv, perct);
        }
    }
}

void Slider::Render()
{
    Scheduler::Push(this, associated.depth);
}

void Slider::Print(float x, float y)
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    SDL_Rect rct = {associated.box.x - Camera::pos.x, associated.box.y - Camera::pos.y, associated.box.w,associated.box.h};
    
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderFillRect(renderer, &rct);
   
    SDL_SetRenderDrawColor(renderer, 127,127,0,255);
    SDL_RenderDrawRect(renderer, &rct);

    SDL_SetRenderDrawColor(renderer, 127,127,0,255);
    rct.w = pct*associated.box.w;
    SDL_RenderFillRect(renderer, &rct);
}

bool Slider::Is(std::string type)
{ return type == "Slider"; }