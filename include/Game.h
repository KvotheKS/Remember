#pragma once
#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include "InputManager.h"
#include "Resources.h"
#include "State.h"
#include "TitleState.h"
#include "PingusState.h"
#include "Effect.h"
#include <iostream>
#include <memory>
#include <string>
#include <ctime>
#include <stack>
#include <cstdlib>

class Game {
    private:
        static std::unique_ptr<Game> instance;
        SDL_Window* window;
        SDL_Renderer* renderer;
        State* storedState;
        std::stack<std::unique_ptr<State>> stateStack;
        int frameStart;
        float dt;
        Game(std::string title, int width, int height);
        void CalculateDeltaTime();
    public:
        ~Game();
        void Run();
        SDL_Renderer* GetRenderer();
        State& GetCurrentState();
        static Game& GetInstance();
        float GetDeltaTime();
        void Push(State* state);

        Game(Game& game) = delete;
        void operator=(Game& game) = delete;
};