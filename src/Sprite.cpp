#include "Sprite.h"
#include "Game.h"
#include "Scheduler.h"

Sprite::Sprite(GameObject& associated) : Component(associated){
    texture = nullptr;
    scale = Vec2(1, 1);
}

Sprite::Sprite(GameObject& associated, std::string file, int frameCount, float frameTime,
               float secondsToSelfDestruct) : Sprite(associated){
    Open(file);
    SetFrameCount(frameCount);
    SetFrameTime(frameTime);
    this->secondsToSelfDestruct = secondsToSelfDestruct;
    selfDestructCount.Restart();
}

Sprite::~Sprite(){
}

void Sprite::Open(std::string file){
    texture = Resources::GetImage(file);
    if(!IsOpen()){
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height) != 0){
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SetClip(0, 0, width, height);

    associated.box.w = width;
    associated.box.h = height;
}

void Sprite::SetClip(int x, int y, int w, int h){
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

void Sprite::Update(float dt){
    if(secondsToSelfDestruct > 0){
        selfDestructCount.Update(dt);
        if(selfDestructCount.Get() > secondsToSelfDestruct)
            associated.RequestDelete();
    }

    timeElapsed += dt;
    if(timeElapsed > frameTime){
        if(currentFrame + 1 == frameCount)
            SetFrame(0);
        else
            SetFrame(currentFrame + 1);
    }
}

void Sprite::Render(){
    Render(associated.box.x - Camera::pos.x, associated.box.y - Camera::pos.y);
}

void Sprite::Render(float x, float y,  float depth){
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = clipRect.w * scale.x;
    dstRect.h = clipRect.h * scale.y;
    // SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture.get(), &this->clipRect, &dstRect,
    //         associated.angleDeg, nullptr, SDL_FLIP_NONE);
    Scheduler::Push(texture.get(), clipRect, dstRect, associated.angleDeg, associated.depth + depth);
}

bool Sprite::Is(std::string type){
    return type == "Sprite";
}

int Sprite::GetWidth(){
    return (width / frameCount) * scale.x;
}

int Sprite::GetHeight(){
    return height * scale.y;
}

bool Sprite::IsOpen(){
    return texture != nullptr;
}

void Sprite::SetScaleX(float scaleX, float scaleY){
    scale.x = !scaleX ? scale.x : scaleX;
    scale.y = !scaleY ? scale.y : scaleY;

    Vec2 center = associated.box.GetCenter();
    associated.box.w = GetWidth();
    associated.box.h = GetHeight();
    associated.box.SetCenter(center.x, center.y);
}

Vec2 Sprite::GetScale(){
    return scale;
}

void Sprite::SetFrame(int frame){
    currentFrame = frame;
    SetClip((GetWidth() * currentFrame / scale.x), 0, (width / frameCount), height);
    timeElapsed = 0;
}

void Sprite::SetFrameCount(int frameCount){
    this->frameCount = frameCount;
    SetFrame(0);
    associated.box.w = GetWidth();
}

void Sprite::SetFrameTime(float frameTime){
    this->frameTime = frameTime;
    timeElapsed = 0;
}