#pragma once
#include "State.h"
#include "StageState.h"
#include "GameObject.h"
#include "Camera.h"
#include "Sprite.h"
#include "Text.h"
#include "InputManager.h"
#include <string>

class MenuState : public State {
    private:
        Music backgroundMusic;
        int menu_pos;
        GameObject* GO_cursor;
        Text* op0;
        Text* op1;
        Text* op2;
        Text* op3;
    public:
        MenuState();
        ~MenuState();
        void LoadAssets();
        void Start();
        void Update(float dt);
        void Render();
        void Pause();
        void Resume();
        void Menu(float dt);
        void Collision();


};