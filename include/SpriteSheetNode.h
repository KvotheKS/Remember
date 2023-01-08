#pragma once
#include "StateMac.h"

class SSNode : public AnimNode
{
public:
    SDL_Rect clip_line;
    SSNode(float actionTime, std::set<int> possibleActions,std::string file, SDL_Rect clip_line, int frameCount, float frameTime, Vec2 scale = Vec2(1,1), bool reverse = false, bool flipped = false);
public:
    // void Update(float dt);
    // void Print(float x = 0.0f, float y = 0.0f, float angleDeg = 0.0f);
    void SetFrame(int id);
    int GetWidth();
    int GetHeight();
};

class BVNode : public SSNode
{
protected:
    bool rev;
public:   
    BVNode(float actionTime, std::set<int> possibleActions,std::string file, SDL_Rect clip_line, int frameCount, float frameTime, Vec2 scale = Vec2(1,1), bool reverse = false, bool flipped = false);
    void Update(float dt);
};