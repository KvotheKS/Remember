#pragma once
#define INCLUDE_SDL
#include "SDL_include.h"
#include <unordered_map>
#include <cstring>

#define LEFT_ARROW_KEY     SDLK_LEFT
#define RIGHT_ARROW_KEY    SDLK_RIGHT
#define UP_ARROW_KEY       SDLK_UP
#define DOWN_ARROW_KEY     SDLK_DOWN
#define W_KEY              SDLK_w
#define A_KEY              SDLK_a
#define S_KEY              SDLK_s
#define D_KEY              SDLK_d
#define Q_KEY              SDLK_q
#define F3_KEY             SDLK_F3
#define ESCAPE_KEY         SDLK_ESCAPE
#define SPACE_KEY          SDLK_SPACE
#define ENTER_KEY          SDLK_RETURN
#define LEFT_MOUSE_BUTTON  SDL_BUTTON_LEFT
#define RIGHT_MOUSE_BUTTON SDL_BUTTON_RIGHT

class InputManager {
    private:
        bool mouseState[6];
        int mouseUpdate[6];
        std::unordered_map<int, bool> keyState;
        std::unordered_map<int, int> keyUpdate;
        bool quitRequested;
        int updateCounter;
        int mouseX;
        int mouseY;
        InputManager();
        ~InputManager();
    public:
        void Update();
        bool KeyPress(int key);
        bool KeyRelease(int key);
        bool IsKeyDown(int key);
        bool MousePress(int button);
        bool MouseRelease(int button);
        bool IsMouseDown(int button);
        int GetMouseX();
        int GetMouseY();
        bool QuitRequested();
        static InputManager& GetInstance();
};