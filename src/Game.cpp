#include "Game.h"
#include "Scheduler.h"
#include "PingusState.h"





std::unique_ptr<Game> Game::instance = nullptr;

Game::Game(std::string title, int width, int height){
    if(instance){
        std::cerr << "The Game has already been instantiated" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cin.tie(0);
    std::cout.tie(0);

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0){
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0){
        std::cerr << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(TTF_Init() != 0){
        std::cerr << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(Mix_Init(MIX_INIT_OGG) == 0){
        std::cerr << Mix_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0){
        std::cerr << Mix_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    Mix_AllocateChannels(32);

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if(window == nullptr){
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr){
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    dt = 0.0;
    frameStart = 0;
    srand(time(NULL));
    storedState = nullptr;
}

Game::~Game(){
    if(storedState != nullptr)
        delete storedState;
    while(!stateStack.empty())
        stateStack.pop();
    Resources::ClearResources();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::CalculateDeltaTime(){
    int prevFrame = frameStart;
    frameStart = SDL_GetTicks();
    dt = (frameStart - prevFrame) / 1000.0;
}

void Game::Run(){
    if(storedState != nullptr){
        stateStack.emplace(storedState);
        GetCurrentState().Start();
        storedState = nullptr;
    }
    else{
        std::cerr << "Initial Game State not specified" << std::endl;
        exit(EXIT_FAILURE);
    }

    while(!stateStack.empty() && !GetCurrentState().QuitRequested()){
        if(GetCurrentState().PopRequested()){
            stateStack.pop();
            Resources::ClearResources();
            if(!stateStack.empty())
                GetCurrentState().Resume();
        }

        if(storedState != nullptr){
            GetCurrentState().Pause();
            stateStack.emplace(storedState);
            GetCurrentState().Start();
            storedState = nullptr;
        }

        CalculateDeltaTime();
        InputManager::GetInstance().Update();
        GetCurrentState().Update(dt);
        GetCurrentState().Render();
        
        Scheduler::Render();

        SDL_RenderPresent(renderer);
        SDL_Delay(15);
        
    }

    while(!stateStack.empty())
        stateStack.pop();
    Resources::ClearResources();
}

SDL_Renderer* Game::GetRenderer(){
    return renderer;
}

State& Game::GetCurrentState(){
    return *stateStack.top();
}

Game& Game::GetInstance(){
    if(!instance)
        instance.reset(new Game("Remember - ♪♫", 1280, 720));
    return *instance;
}

float Game::GetDeltaTime(){
    return dt;
}

void Game::Push(State *state){
    storedState = state;
}