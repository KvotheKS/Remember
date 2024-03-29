#pragma once
#define INCLUDE_SDL
#include "SDL_include.h"
#include "GameObject.h"
#include "Component.h"
#include "Resources.h"
#include "Camera.h"
#include "Timer.h"
#include "Vec2.h"
#include <string>
#include <memory>
#include <iostream>

#define SPRITE_T "Sprite"

class Sprite : public GameObject {
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
        Timer selfDestructCount;
        bool fliped;
        
    public:
        Uint8 r = 255, g = 255, b = 255, a= 255;

  
        Sprite(GameObject& associated);
        Sprite(GameObject& associated, std::string file, int frameCount = 1,
               float frameTime = 1, float secondsToSelfDestruct = 0);
        ~Sprite();
        void Open(std::string file);
        void SetClip(int x, int y, int w, int h);
        void Update(float dt);

        
        void Render();
        void Render(float x, float y);
        void Print(float x=-1, float y=-1);
        bool Is(std::string type);
        bool Is(C_ID);

        int GetWidth();
        int GetHeight();
        
        bool IsOpen();
        void SetSize(float sizeW, float sizeH);
        void SetScaleX(float scaleX, float scaleY);
        Vec2 GetScale();

        void SetFrame(int frame);
        void SetFrameCount(int frameCount);
        void SetFrameTime(float frameTime);
        void SetTint(Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255);
        bool GetFliped();
        void SetFliped(bool value);
};