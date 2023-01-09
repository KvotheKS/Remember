#include "GameBoxT.h"
#include "Scheduler.h"

GameBoxT::GameBoxT(GameObject& associated, Uint8 r, Uint8 g, Uint8 b)
    : GameObject(associated)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void GameBoxT::Render()
{
    Scheduler::Push(this, depth);
}

void GameBoxT::Print(float x, float y)
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    Rect fbox = relative + box;
    SDL_Rect rct = {fbox.x - Camera::pos.x, fbox.y - Camera::pos.y, fbox.w,fbox.h};
    SDL_SetRenderDrawColor(renderer, r,g,b,255);
    SDL_RenderFillRect(renderer, &rct);
}

bool GameBoxT::Is(std::string type)
{return Is() == type; }

std::string GameBoxT::Is()
{return "GameBoxT"; }

bool GameBoxT::Is(C_ID type)
{ return type == C_ID::GameBoxT; }