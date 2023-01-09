#pragma once
#include "StateMac.h"

class SSNode : public AnimNode
{
public:
    SDL_Rect clip_line;
    SSNode(std::string file, SDL_Rect clip_line, int frameCount, float frameTime, Vec2 scale = Vec2(1,1), bool reverse = false, bool flipped = false);
public:
    // void Update(float dt);
    // void Print(float x = 0.0f, float y = 0.0f, float angleDeg = 0.0f);
    void SetFrame(int id);
    int GetWidth();
    int GetHeight();
};