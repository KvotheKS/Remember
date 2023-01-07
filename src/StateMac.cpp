#include "StateMac.h"
#include "Resources.h"
#include "Game.h"
#include "Vec2.h"
#include "Scheduler.h"

AnimNode::AnimNode(std::string file, int frameCount, float frameTime, Vec2 scale, bool reverse, bool flipped)
{
    texture = nullptr;
    this->scale = scale;
    Open(file);
    SetFrameCount(frameCount);
    SetFrameTime(frameTime);
    fliped = flipped;
    this->reverse = reverse;
}

void AnimNode::Open(std::string file){
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
}

void AnimNode::SetClip(int x, int y, int w, int h){
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

void AnimNode::Print(float x, float y, float angle)
{
    SDL_Rect dstRect;
    dstRect.x = x - (clipRect.w * scale.x/2)+clipRect.w/2;
    dstRect.y = y - (clipRect.h * scale.y/2)+clipRect.h/2;
 
    dstRect.w = clipRect.w * scale.x;
    dstRect.h = clipRect.h * scale.y;

    SDL_SetTextureColorMod(texture.get(), r, g, b);

    SDL_RendererFlip flip_val = SDL_FLIP_NONE;
    if(fliped) {
        flip_val = SDL_FLIP_HORIZONTAL;
    }
    SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture.get(), &this->clipRect, &dstRect,
            angle, nullptr, flip_val);
            
    SDL_SetTextureColorMod(texture.get(), 255,255,255);
}

void AnimNode::Update(float dt){
    timeElapsed += dt;
    if(timeElapsed > frameTime){
        if(reverse)
        {
            if(currentFrame == 0)
                finished = true;
            SetFrame(currentFrame - 1);
            return;
        }

        if(currentFrame + 1 == frameCount)
        {
            finished = true;
            SetFrame(0);
        }
        else
            SetFrame(currentFrame + 1);
        timeElapsed -= frameTime;
    }
}

void AnimNode::Reset()
{
    finished = false;
    SetFrame(reverse*(frameCount - 1));
}

int AnimNode::GetWidth(){
    return (width / frameCount) * scale.x;
}

int AnimNode::GetHeight(){
    return height * scale.y;
}

bool AnimNode::IsOpen(){
    return texture != nullptr;
}

void AnimNode::SetScaleX(float scaleX, float scaleY){
    scale.x = !scaleX ? scale.x : scaleX;
    scale.y = !scaleY ? scale.y : scaleY;
    
}

Vec2 AnimNode::GetScale(){
    return scale;
}

void AnimNode::SetFrame(int frame){
    currentFrame = frame;
    SetClip((GetWidth() * currentFrame / scale.x), 0, (width / frameCount), height);
    timeElapsed = 0;
}

void AnimNode::SetFrameCount(int frameCount){
    this->frameCount = frameCount;
    SetFrame(0);
    // associated.box.w = GetWidth();
}

void AnimNode::SetFrameTime(float frameTime){
    this->frameTime = frameTime;
    timeElapsed = 0;
}

void AnimNode::SetTint(Uint8 r, Uint8 g, Uint8 b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}
bool AnimNode::GetFliped(){
    return fliped;
}
void AnimNode::SetFliped(bool value){
    fliped = value;
}

bool AnimNode::IsDone()
{
    return finished;
}

StateMachine::StateMachine(GameObject& associated)
    : GameObject(associated), __curr(0)
{}

void StateMachine::Update(float dt)
{
    if(!states.empty())
    {
        states[__curr]->Update(dt);
        if(states[__curr]->IsDone())
            ChangeState(transitions[__curr]);
    }
}

void StateMachine::Render()
{
    float x = associated.box.x - Camera::pos.x;
    float y = associated.box.y - Camera::pos.y;
    Scheduler::Push(this, associated.depth + depth, x, y);
}

void StateMachine::Print(float x, float y)
{
    states[__curr]->Print(x,y,associated.angleDeg);
}

void StateMachine::AddTransition(int from, int to)
{
    auto it = transitions.find(from);
    if(it != transitions.end())
        it->second = to;
    else
        transitions.insert({from, to});
}

void StateMachine::AddNode(int id, AnimNode* anm)
{
    states.insert({id, std::unique_ptr<AnimNode>(anm)});
    transitions.insert({id,id});
}

std::pair<const int, AnimNode*> StateMachine::GetCurrent()
{
    return {__curr, states[__curr].get()};
}

void StateMachine::CenterBox(Rect& bx)
{
    auto [__, cr] = GetCurrent();

    Vec2 center = bx.GetCenter();
    bx.w = cr->GetWidth();
    bx.h = cr->GetHeight();
    bx.SetCenter(center.x, center.y);
}

std::unordered_map<int, std::unique_ptr<AnimNode>>& StateMachine::GetStates()
{ return states; }

void StateMachine::ChangeState(int newSt)
{
    auto it = states.find(__curr);
    if(it != states.end()) 
        it->second->Reset();
    __curr = newSt;
    CenterBox(associated.box);
}

bool StateMachine::Is(std::string type)
{return "StateMachine";}