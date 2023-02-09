#include "Camera.h"
#include "Game.h"

#include <iostream>
#include <limits>

GameObject* Camera::focus = nullptr;
CameraFunction Camera::fnc = &Camera::FollowTarget;
Vec2 Camera::pos = Vec2();
Vec2 Camera::speed = Vec2();
Vec2 Camera::lpos = Vec2();
int Camera::width = 0, Camera::height = 0;
Rect Camera::Bounds = Rect(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
std::pair<ConditionalLambda, CameraFunction> Camera::transition = std::make_pair(Camera::False, Camera::Stationary);

void Camera::Follow(GameObject* newFocus){
    focus = newFocus;
}

void Camera::Unfollow(){
    focus = nullptr;
}

void Camera::SetCameraTransition(ConditionalLambda condfnc, CameraFunction camerafnc)
{
    SDL_GetRendererOutputSize(Game::GetInstance().GetRenderer(), &width, &height);
    Camera::transition = std::make_pair(condfnc, camerafnc);
}

void Camera::SetCameraFunction(CameraFunction fnc)
{
    SDL_GetRendererOutputSize(Game::GetInstance().GetRenderer(), &width, &height);

    Camera::fnc = fnc;
}

void Camera::Update(float dt)
{
    if(transition.first())
        SetCameraFunction(transition.second);
    Camera::fnc(dt);
}

bool Camera::False()
{ return false; }

void Camera::Stationary(float dt)
{}

void Camera::FollowTarget(float dt){
    lpos = pos;

    if(focus != nullptr){
        // int width, height;
        Vec2 focusCenter = focus->box.GetCenter();
        


        /*por algum motivo com floor o quadrado n treme, mas o penguin treme e visse versa*/

        pos.x = floor(focusCenter.x) - width/2;
        pos.y = floor(focusCenter.y) - height/2;

        pos.x = min(Bounds.x + Bounds.w - width, max(Bounds.x, pos.x));
        pos.y = min(Bounds.y + Bounds.h - height, max(Bounds.y, pos.y));
        
        //--- update objetos dependentes da posição da camera ---//
        State& state = Game::GetInstance().GetCurrentState();
        for(unsigned i = 0; i < state.cameraFollowerObjectArray.size(); i++)
            state.cameraFollowerObjectArray[i]->UpdateNodes(dt);
        
    }
}

void Camera::ControlCamera(float dt)
{
    int linearSpeed = 325;
    InputManager& inManager = InputManager::GetInstance();
    Vec2 direction = Vec2();

    if(inManager.IsKeyDown(LEFT_ARROW_KEY)){ 
        direction -= Vec2(1, 0);
    }
    if(inManager.IsKeyDown(RIGHT_ARROW_KEY))
        direction += Vec2(1, 0);
    if(inManager.IsKeyDown(UP_ARROW_KEY))
        direction -= Vec2(0, 1);
    if(inManager.IsKeyDown(DOWN_ARROW_KEY))
        direction += Vec2(0, 1);
    speed = direction.Normalize() * linearSpeed;

    pos += speed * dt;
}