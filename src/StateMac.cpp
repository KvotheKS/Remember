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
    finished = false;
    rendered = true;
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
    dstRect.x = x ;
    dstRect.y = y;
    // dstRect.x = x - (clipRect.w * scale.x/2)+clipRect.w/2;
    // dstRect.y = y - (clipRect.h * scale.y/2)+clipRect.h/2;
 
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
    if(timeElapsed > frameTime){
        if(reverse)
        {
            if(currentFrame == 0)
                finished = true;
            else
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

void AnimNode::Render()
{ rendered = true; }

void AnimNode::Reset()
{
    timeElapsed = 0.0f;
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
{ return finished; }

StateMachine::StateMachine(GameObject& associated)
    : GameObject(associated), __curr(0), justFinished(false)
{}

void StateMachine::Update(float dt)
{
    if(!states.empty())
    {
        // std::cout << "1\n";
        auto it = states.find(__curr);
        // std::cout << it->second->rendered;
        if(it->second->rendered)
        {
            // std::cout << "tt1\n";1
            it->second->timeElapsed+=dt;
            it->second->Update(dt);
            // std::cout << "2\n";
        }
        
        if(it->second->IsDone())
        {
            // std::cout << "3\n";
            ChangeState(transitions[__curr]);
            justFinished = true;
        }
        else justFinished = false;
    }
}

void StateMachine::Render()
{
    states[__curr]->Render();
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

bool StateMachine::IsDone()
{ return justFinished; }

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
    bool flip = false;
    if(it != states.end())
    {
        it->second->Reset();
        flip = it->second->fliped;
    }
    __curr = newSt;
    it = states.find(__curr);
    it->second->rendered = false;
    it->second->fliped = flip;
    CenterBox(associated.box);
}

bool StateMachine::Is(std::string type)
{return type == "StateMachine"; }

bool StateMachine::Is(C_ID type)
{ return type == C_ID::StateMachine; }