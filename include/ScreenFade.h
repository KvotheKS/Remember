#pragma once
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"
#include "Resources.h"
#include "Component.h"
#include "Timer.h"
#include "GameObject.h"
#include <string>
#include <memory>
#include <iostream>


enum FSTATE
{
    F_IN,SOLID,F_OUT
};

class ScreenFade : public GameObject {
    private:

        SDL_Rect clipRect;
        SDL_Color color;
        Timer duration;
        Timer fadeTimer;
        int local_state;

        float fadeinTime;
        float fadeoutTime;

    public:
     
        ScreenFade(GameObject& associated, float fadeinTime, float fadeoutTime,float duration);
        ~ScreenFade();
        void Open(std::string file);
        void SetClip(int x, int y, int w, int h);
        void SetColor(Uint8 r , Uint8 g ,Uint8 b, Uint8 a);
        void Update(float dt);
        void Render();
        void Render(float x, float y);
        void Print(float x=-1, float y=-1);
        bool Is(std::string type);
        bool Is(C_ID);
};