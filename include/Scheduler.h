#pragma once
#define INCLUDE_SDL
#include "SDL_include.h"
#include <memory>
#include <string>
#include <vector>

struct info
{
    SDL_Texture* texture;
    SDL_Rect clip, dst;
    float angle, depth;
};

class Scheduler
{
    static std::vector<info> textureVec;
public:
    static void Render();
    static void Push(SDL_Texture* texture, SDL_Rect& clip, SDL_Rect& dst, float angle = 0.0f, float depth = 0.0f );
    static void Clear();
};