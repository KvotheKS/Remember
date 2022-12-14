#include "Camera.h"
#include "Game.h"

GameObject* Camera::focus = nullptr;
Vec2 Camera::pos = Vec2();
Vec2 Camera::speed = Vec2();

void Camera::Follow(GameObject* newFocus){
    focus = newFocus;
}

void Camera::Unfollow(){
    focus = nullptr;
}

void Camera::Update(float dt){
    if(focus != nullptr){
        int width, height;
        Vec2 focusCenter = focus->box.GetCenter();

        SDL_GetRendererOutputSize(Game::GetInstance().GetRenderer(), &width, &height);
        pos.x = focusCenter.x - width/2;
        pos.y = focusCenter.y - height/2;
    }
    else{
        int linearSpeed = 325;
        InputManager& inManager = InputManager::GetInstance();
        Vec2 direction = Vec2();

        if(inManager.IsKeyDown(LEFT_ARROW_KEY))
            direction -= Vec2(1, 0);
        if(inManager.IsKeyDown(RIGHT_ARROW_KEY))
            direction += Vec2(1, 0);
        if(inManager.IsKeyDown(UP_ARROW_KEY))
            direction -= Vec2(0, 1);
        if(inManager.IsKeyDown(DOWN_ARROW_KEY))
            direction += Vec2(0, 1);
        speed = direction.Normalize() * linearSpeed;

        pos += speed * dt;
    }
}