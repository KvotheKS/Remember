#include "Sprite.h"
#include "Game.h"
#include "Scheduler.h"

Sprite::Sprite(GameObject& associated) : GameObject(associated){
    texture = nullptr;
    scale = Vec2(1, 1);
}

Sprite::Sprite(GameObject& associated, std::string file, int frameCount, float frameTime,
               float secondsToSelfDestruct) : Sprite(associated){
    Open(file);
    SetFrameCount(frameCount);
    SetFrameTime(frameTime);
    selfDestructCount.SetFinish(secondsToSelfDestruct);
    fliped = false;
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
    if(selfDestructCount.Update(dt))
        associated.RequestDelete();


    timeElapsed += dt;
    if(timeElapsed > frameTime){
        if(currentFrame + 1 == frameCount)
            SetFrame(0);
        else
            SetFrame(currentFrame + 1);
    }

   
}



void Sprite::Render()
{
    float paralax_mul;
    switch((int)associated.depth){//deepts -1 to -4 are afected  paralax
        case -1:
            paralax_mul = 0.925;
            break;
        case -2:
            paralax_mul = 0.85;
            break;
        case -3:
            paralax_mul = 0.775;
            break;
        case -4:
            paralax_mul = 0.7;
            break;
        case -10:
            paralax_mul = 0;
            break;
        default:
            paralax_mul = 1;
            break;
    }

    Render(associated.box.x - Camera::pos.x*paralax_mul, associated.box.y - Camera::pos.y*paralax_mul);

    Render(associated.box.x + relative.x - Camera::pos.x*paralax_mul, associated.box.y + relative.y - Camera::pos.y);

}

void Sprite::Render(float x, float y){
    
    // SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture.get(), &this->clipRect, &dstRect,
    //         associated.angleDeg, nullptr, SDL_FLIP_NONE);
    Scheduler::Push(this, associated.depth + depth, x, y);
}

void Sprite::Print(float x, float y)
{
    SDL_Rect dstRect;

    // dstRect.x = x - (clipRect.w * scale.x/2)+clipRect.w/2;
    // dstRect.y = y - (clipRect.h * scale.y/2)+clipRect.h/2;
    dstRect.x = x; dstRect.y = y;
    dstRect.w = clipRect.w * scale.x;
    dstRect.h = clipRect.h * scale.y;
    // if(associated.GetComponent(C_ID::Attack))
    //     std::cout << "SPRITE" << dstRect.x << ' ' << dstRect.y << ' ' << dstRect.w << ' ' << dstRect.h << '\n';

    SDL_SetTextureColorMod(texture.get(),r , g, b);
    SDL_SetTextureAlphaMod(texture.get(), a); // transparencia
    SDL_RendererFlip flip_val = SDL_FLIP_NONE;
    if(fliped) {
        flip_val = SDL_FLIP_HORIZONTAL;
    }
    SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture.get(), &this->clipRect, &dstRect,
            associated.angleDeg, nullptr,flip_val);

    
    
            
    SDL_SetTextureColorMod(texture.get(), 255,255,255);
}

bool Sprite::Is(std::string type){
    return type == "Sprite";
}

bool Sprite::Is(C_ID type)
{ return type == C_ID::Sprite; }

int Sprite::GetWidth(){
    return (width / frameCount) * scale.x;
}

int Sprite::GetHeight(){
    return height * scale.y;
}

bool Sprite::IsOpen(){
    return texture != nullptr;
}

void Sprite::SetSize(float sizeW, float sizeH)
{
    SetScaleX(sizeW/width, sizeH/height);
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

void Sprite::SetTint(Uint8 r, Uint8 g, Uint8 b,Uint8 a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}
bool Sprite::GetFliped(){
    return fliped;
}
void Sprite::SetFliped(bool value){
    fliped = value;
}