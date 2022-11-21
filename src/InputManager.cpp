#include "InputManager.h"

InputManager::InputManager() : quitRequested(false), updateCounter(0), mouseX(0), mouseY(0){
    memset(mouseState, false, 6 * sizeof(bool));
    memset(mouseUpdate, 0, 6 * sizeof(int));
}

InputManager::~InputManager(){
}

void InputManager::Update(){
    SDL_Event event;
    SDL_GetMouseState(&mouseX, &mouseY);
    quitRequested = false;
    updateCounter++;

    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT)
            quitRequested = true;
        if(event.type == SDL_MOUSEBUTTONDOWN){
            mouseState[event.button.button] = true;
            mouseUpdate[event.button.button] = updateCounter;
        }
        if(event.type == SDL_MOUSEBUTTONUP){
            mouseState[event.button.button] = false;
            mouseUpdate[event.button.button] = updateCounter;
        }
        if(event.type == SDL_KEYDOWN && !event.key.repeat){
            keyState[event.key.keysym.sym] = true;
            keyUpdate[event.key.keysym.sym] = updateCounter;
        }
        if(event.type == SDL_KEYUP){
            keyState[event.key.keysym.sym] = false;
            keyUpdate[event.key.keysym.sym] = updateCounter;
        }
    }
}

bool InputManager::KeyPress(int key){
    return keyUpdate[key] == updateCounter ? keyState[key] : false;
}

bool InputManager::KeyRelease(int key){
    return keyUpdate[key] == updateCounter ? !keyState[key] : false;
}

bool InputManager::IsKeyDown(int key){
    return keyState[key];
}

bool InputManager::MousePress(int button){
    return mouseUpdate[button] == updateCounter ? mouseState[button] : false;
}

bool InputManager::MouseRelease(int button){
    return mouseUpdate[button] == updateCounter ? !mouseState[button] : false;
}

bool InputManager::IsMouseDown(int button){
    return mouseState[button];
}

int InputManager::GetMouseX(){
    return mouseX;
}

int InputManager::GetMouseY(){
    return mouseY;
}

bool InputManager::QuitRequested(){
    return quitRequested;
}

InputManager& InputManager::GetInstance(){
    static InputManager inManager;
    return inManager;
}