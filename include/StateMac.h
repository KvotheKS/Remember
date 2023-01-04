#pragma once
#define INCLUDE_SDL
#include "SDL_include.h"
#include "GameObject.h"
#include <unordered_map>
#include <memory>

class AnimNode
{
public:
    std::shared_ptr<SDL_Texture> texture;
    int width;
    int height;
    SDL_Rect clipRect;
    Vec2 scale;
    int frameCount;
    int currentFrame;
    float timeElapsed;
    float frameTime;
    bool fliped;
    bool reverse;
    bool finished = false;
    Uint8 r=255, g=255, b=255;

public:
    AnimNode(std::string file, int frameCount, float frameTime, Vec2 scale = Vec2(1,1), bool reverse = false, bool flipped = false);

public:
    virtual void Update(float dt);
    virtual void Print(float x = 0.0f, float y = 0.0f, float angle = 0.0f);
    virtual int GetWidth();
    virtual int GetHeight();
    virtual void Reset();
    void Open(std::string);
    bool IsOpen();

    void SetScaleX(float scaleX, float scaleY);
    Vec2 GetScale();

    bool IsDone();
    void SetClip(int,int,int,int);
    virtual void SetFrame(int frame);
    void SetFrameCount(int frameCount);
    void SetFrameTime(float frameTime);
    void SetTint(Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);
    bool GetFliped();
    void SetFliped(bool value);
};

class StateMachine : public GameObject
{
    int __curr = 0;
    std::unordered_map<int,std::shared_ptr<AnimNode>> states;
    std::unordered_map<int,int> transitions;

public:
    StateMachine(GameObject& associated);

public:
    void Update(float dt);
    void Render();
    void Print(float x, float y);

public:
    void AddTransition(int,int);
    void AddNode(int,AnimNode*);
    AnimNode* CurrentState();
    void ChangeState(int);
    void CenterBox(Rect&);
    std::unordered_map<int, std::shared_ptr<AnimNode>>& GetStates();
};